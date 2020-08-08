#include "../../common/cppex/include/scope_exit.h"
#include "../../common/cppex/include/unused.h"
#include "../../common/trace/include/trace.h"

#include <array>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <zmq.hpp>
#include <zmq_addon.hpp>

std::string generate_identity();

int main()
{
    melinda::trace::trace_options trace_config(fs::path("../log"),
        fs::path("client"));
    trace_config.level = melinda::trace::trace_level::info;

    melinda::trace::trace_handle trace_handle =
        melinda::trace::initialize_trace(trace_config);
    ON_SCOPE_EXIT(melinda::trace::close_trace(trace_handle));

    constexpr std::size_t const max_identity_length = 255;
    std::string const identity = generate_identity();

    zmq::context_t ctx;
    zmq::socket_t socket = zmq::socket_t(ctx, zmq::socket_type::req);

    socket.setsockopt(ZMQ_LINGER, 0); // Close the socket immediately
    socket.setsockopt(ZMQ_SNDTIMEO,
        10 * 1000); // Set the timeout for sending requests to 10 seconds
    socket.setsockopt(ZMQ_ROUTING_ID,
        identity.c_str(),
        std::min(max_identity_length,
            identity.length())); // Identify to the server with a unique name

    constexpr char const* const address = "tcp://localhost:22365";
    try
    {
        MELINDA_TRACE_ALWAYS(trace_handle,
            "Client will connect to {}.",
            address);
        socket.connect(address);
    }
    catch (zmq::error_t const& ex)
    {
        MELINDA_TRACE_FATAL(trace_handle,
            "Can't connect to {}. Reason: ZMQ-{}: ",
            ex.num(),
            ex.what());
        std::terminate();
    }
    ON_SCOPE_EXIT(socket.disconnect(address));

    while (true)
    {
        zmq::message_t request =
            zmq::message_t(address, sizeof("tcp://localhost:22365"));
        try
        {
            MELINDA_TRACE_INFO(trace_handle,
                "Sending request of {} bytes to {}.",
                request.size(),
                address);
            zmq::send_result_t const result =
                socket.send(request, zmq::send_flags::none);
            if (!result)
            {
                MELINDA_TRACE_ERROR(trace_handle,
                    "Can't send request to {}, server might be unreachable",
                    "");
                continue;
            }
        }
        catch (zmq::error_t const& ex)
        {
            MELINDA_TRACE_FATAL(trace_handle,
                "Unexpected error while sending request. Reason: ZMQ-{}: ",
                ex.num(),
                ex.what());
        }

        zmq::message_t response;
        try
        {
            zmq::recv_result_t const result =
                socket.recv(response); // TODO-JK this is blocking indefinately
            if (result)
            {
                MELINDA_TRACE_INFO(trace_handle,
                    "Received response of {} bytes in size from {}",
                    result.value(),
                    address);
            }
            else
            {
                continue;
            }
        }
        catch (zmq::error_t const& ex)
        {
            MELINDA_TRACE_FATAL(trace_handle,
                "Unexpected error while receiving response. Reason: ZMQ-{}: ",
                ex.num(),
                ex.what());
        }
    }
}

std::string generate_identity()
{
    boost::uuids::random_generator generator;
    std::string const uuid = to_string(generator());

    std::array<char, HOST_NAME_MAX + 1> buffer = {};
    gethostname(buffer.data(), HOST_NAME_MAX);

    return uuid + "_" + buffer.data();
}
