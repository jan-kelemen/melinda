#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include <unistd.h>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "ncprot_client.h"
#include "scope_exit.h"
#include "trace.h"
#include "unused.h"
#include "wire_generated.h"

int main()
{
    flatbuffers::FlatBufferBuilder builder;
    auto binary_content =
        mel::network::CreatemessageDirect(builder, 5, "Hello");
    builder.Finish(binary_content);

    size_t size = builder.GetSize();
    uint8_t* data = builder.GetBufferPointer();

    mel::trace::trace_options trace_config(fs::path("../log"),
        fs::path("client"));
    trace_config.level = mel::trace::trace_level::info;

    mel::trace::initialize_process_trace_handle(
        mel::trace::create_trace_handle(trace_config));

    mel::cppex::result<std::string> const identity =
        mel::ncprot::generate_identity();
    if (!identity)
    {
        MEL_TRACE_ERROR("Unable to create client identity: {}!",
            static_cast<std::error_code>(identity).message());
        std::terminate();
    }
    std::string const& raw_identity = static_cast<std::string const&>(identity);

    zmq::context_t ctx;
    zmq::socket_t socket = zmq::socket_t(ctx, zmq::socket_type::req);

    socket.setsockopt(ZMQ_LINGER, 0); // Close the socket immediately
    socket.setsockopt(ZMQ_SNDTIMEO,
        10 * 1000); // Set the timeout for sending requests to 10 seconds
    socket.setsockopt(ZMQ_ROUTING_ID,
        raw_identity.c_str(),
        raw_identity.length()); // Identify to the server with a unique name

    constexpr char const* const address = "tcp://localhost:22365";
    try
    {
        MEL_TRACE_ALWAYS("Client will connect to {}.", address);
        socket.connect(address);
    }
    catch (zmq::error_t const& ex)
    {
        MEL_TRACE_FATAL("Can't connect to {}. Reason: ZMQ-{}: ",
            ex.num(),
            ex.what());
        std::terminate();
    }
    ON_SCOPE_EXIT(socket.disconnect(address));

    while (true)
    {
        zmq::message_t request = zmq::message_t(data, size);
        try
        {
            MEL_TRACE_INFO("Sending request of {} bytes to {}.",
                request.size(),
                address);
            zmq::send_result_t const result =
                socket.send(request, zmq::send_flags::none);
            if (!result)
            {
                MEL_TRACE_ERROR(
                    "Can't send request to {}, server might be unreachable");
                continue;
            }
        }
        catch (zmq::error_t const& ex)
        {
            MEL_TRACE_FATAL(
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
                MEL_TRACE_INFO("Received response of {} bytes in size from {}",
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
            MEL_TRACE_FATAL(
                "Unexpected error while receiving response. Reason: ZMQ-{}: ",
                ex.num(),
                ex.what());
        }
    }
}
