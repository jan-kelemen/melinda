#include "../../common/cppex/include/scope_exit.h"
#include "../../common/trace/include/trace.h"

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

    constexpr int linger = 0;
    socket.setsockopt(ZMQ_LINGER, linger);
    socket.setsockopt(ZMQ_ROUTER_MANDATORY, 1);

    constexpr char const* const address = "tcp://*:22365";
    try
    {
        MELINDA_TRACE_ALWAYS(trace_handle, "Binding to {}.", address);
        socket.bind(address);
        MELINDA_TRACE_ALWAYS(trace_handle, "Bound to {}.", address);
    }
    catch (zmq::error_t const& e)
    {
        MELINDA_TRACE_FATAL(trace_handle,
            "Can't bind to {}. Reason: ZMQ-{}:",
            e.num(),
            e.what());
    }
    ON_SCOPE_EXIT(socket.unbind(address));

    zmq::message_t message = zmq::message_t(1024);
    zmq::message_t identity = zmq::message_t(1024);
    zmq::message_t empty = zmq::message_t(1024);
    while (true)
    {
        if (socket.recv(identity))
        {
            if (socket.recv(empty))
            {
                if (socket.recv(message))
                {
                    zmq::message_t send =
                        zmq::message_t(address, sizeof("tcp://*:22365"));

                    std::cout
                        << "Recevied message: " << message.to_string_view()
                        << '\n';
                    socket.send(identity, zmq::send_flags::sndmore);
                    socket.send(empty, zmq::send_flags::sndmore);
                    socket.send(send, zmq::send_flags::none);
                }
            }
        }
    }
}
