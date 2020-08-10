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
    mel::trace::trace_options trace_config(fs::path("../log"),
        fs::path("server"));
    trace_config.level = mel::trace::trace_level::info;

    mel::trace::initialize_process_trace_handle(
        mel::trace::create_trace_handle(trace_config));

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
        MEL_TRACE_ALWAYS("Server will be registered on '{}' endpoint.",
            address);
        socket.bind(address);
    }
    catch (zmq::error_t const& e)
    {
        MEL_TRACE_FATAL("Can't bind to {}. Reason: ZMQ-{}:", e.num(), e.what());
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
                MEL_TRACE_INFO(
                    "Received {} messages of {} bytes in size from {}.",
                    request.size(),
                    result.value(),
                    request[0].to_string_view());
            }
        }
        catch (zmq::error_t const& ex)
        {
            MEL_TRACE_FATAL(
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
            MEL_TRACE_INFO("Sending response of {} bytes to {}.",
                response[2].size(),
                response[0].to_string_view());
            zmq::send_result_t const result = zmq::send_multipart(socket,
                std::move(response),
                zmq::send_flags::dontwait);
            if (!result)
            {
                MEL_TRACE_ERROR("Can't send response");
            }
        }
        catch (zmq::error_t const& ex)
        {
            if (ex.num() == EHOSTUNREACH)
            {
                MEL_TRACE_WARN("Client has disconnected or is not known.");
            }
            else
            {
                MEL_TRACE_FATAL(
                    "Unexpected error while receiving request. Reason: ZMQ-{}:",
                    ex.num(),
                    ex.what());
            }
        }
    }
}
