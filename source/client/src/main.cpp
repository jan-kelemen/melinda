#include "../../common/cppex/include/scope_exit.h"
#include "../../common/trace/include/trace.h"

#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <zmq.hpp>
#include <zmq_addon.hpp>

int main()
{
    melinda::trace::trace_options trace_config(fs::path("../log"),
        fs::path("client"));
    trace_config.level = melinda::trace::trace_level::info;

    melinda::trace::trace_handle trace_handle =
        melinda::trace::initialize_trace(trace_config);
    ON_SCOPE_EXIT(melinda::trace::close_trace(trace_handle));

    zmq::context_t ctx;
    zmq::socket_t socket = zmq::socket_t(ctx, zmq::socket_type::req);

    constexpr int linger = 0;
    socket.setsockopt(ZMQ_LINGER, linger);

    constexpr char const* const address = "tcp://localhost:22365";
    try
    {
        MELINDA_TRACE_ALWAYS(trace_handle, "Connecting to {}.", address);
        socket.connect(address);
        MELINDA_TRACE_ALWAYS(trace_handle, "Connected to {}.", address);
    }
    catch (zmq::error_t const& e)
    {
        MELINDA_TRACE_FATAL(trace_handle,
            "Can't connect to {}. Reason: ZMQ-{}:",
            e.num(),
            e.what());
    }
    ON_SCOPE_EXIT(socket.disconnect(address));

    zmq::message_t message = zmq::message_t(1024);

    while (true)
    {
        zmq::message_t send =
            zmq::message_t(address, sizeof("tcp://localhost:22365"));
        socket.send(send, zmq::send_flags::none);
        if (socket.recv(message))
        {
            std::cout << "Recevied message: " << message.to_string_view()
                      << '\n';
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}
