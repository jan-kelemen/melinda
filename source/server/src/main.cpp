#include "../../common/cppex/include/scope_exit.h"
#include "../../common/trace/include/trace.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
#include <zmq.hpp>
#include <zmq_addon.hpp>

int main()
{
    melinda::trace::trace_options trace_config(fs::path("../log"),
        fs::path("server"));
    trace_config.level = melinda::trace::trace_level::info;

    melinda::trace::trace_handle trace_handle =
        melinda::trace::initialize_trace(trace_config);
    ON_SCOPE_EXIT(melinda::trace::close_trace(trace_handle));

    zmq::context_t ctx;
    zmq::socket_t socket = zmq::socket_t(ctx, zmq::socket_type::router);

    socket.setsockopt(ZMQ_LINGER, 0); // Close the socket immediately
    socket.setsockopt(ZMQ_ROUTER_MANDATORY,
        1); // Report host unreachable errors if the reply can't be routed
    socket.setsockopt(ZMQ_SNDTIMEO,
        10 * 1000); // Set the timeout for sending replies to 10 seconds

    constexpr char const* const address = "tcp://*:22365";
    try
    {
        MELINDA_TRACE_ALWAYS(trace_handle,
            "Server will be registered on '{}' endpoint.",
            address);
        socket.bind(address);
    }
    catch (zmq::error_t const& e)
    {
        MELINDA_TRACE_FATAL(trace_handle,
            "Can't bind to {}. Reason: ZMQ-{}:",
            e.num(),
            e.what());
        std::terminate();
    }
    ON_SCOPE_EXIT(socket.unbind(address));

    while (true)
    {
        std::vector<zmq::message_t> request;
        try
        {
            zmq::recv_result_t const result = zmq::recv_multipart(socket,
                std::back_inserter(request),
                zmq::recv_flags::dontwait);
            if (result)
            {
                // Process the message, otherwise if result is empty then no
                // messages were queued on the socket
                MELINDA_TRACE_INFO(trace_handle,
                    "Received {} messages of {} bytes in size from {}.",
                    request.size(),
                    result.value(),
                    request[0].to_string_view());
            }
        }
        catch (zmq::error_t const& ex)
        {
            MELINDA_TRACE_FATAL(trace_handle,
                "Unexpected error while receiving request. Reason: ZMQ-{}:",
                ex.num(),
                ex.what());
        }

        if (request.empty())
        {
            continue;
        }

        try
        {
            std::vector<zmq::message_t> response;
            response.emplace_back(std::move(request[0]));
            response.emplace_back();
            response.emplace_back(address, sizeof("tcp://*:22365"));
            MELINDA_TRACE_INFO(trace_handle,
                "Sending response of {} bytes to {}.",
                response[2].size(),
                response[0].to_string_view());
            zmq::send_result_t const result = zmq::send_multipart(socket,
                std::move(response),
                zmq::send_flags::dontwait);
            if (!result)
            {
                MELINDA_TRACE_ERROR(trace_handle, "Can't send response", "");
            }
        }
        catch (zmq::error_t const& ex)
        {
            if (ex.num() == EHOSTUNREACH)
            {
                MELINDA_TRACE_WARN(trace_handle,
                    "Client has disconnected or is not known.",
                    "");
            }
            else
            {
                MELINDA_TRACE_FATAL(trace_handle,
                    "Unexpected error while receiving request. Reason: ZMQ-{}:",
                    ex.num(),
                    ex.what());
            }
        }
    }
}
