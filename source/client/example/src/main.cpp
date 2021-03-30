#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

#include "ncprot_client.h"
#include "ncprot_serialization.h"
#include "scope_exit.h"
#include "trace.h"

int main()
{
    mel::trace::trace_options trace_config(fs::path("../log"),
        fs::path("example_client"));
    trace_config.level = mel::trace::trace_level::info;

    mel::trace::initialize_process_trace_handle(
        mel::trace::create_trace_handle(trace_config));

    zmq::context_t ctx;
    constexpr char const* const address = "tcp://localhost:22365";

    mel::cppex::result<zmq::socket_t> connect_result =
        mel::ncprot::client::connect(ctx, address);
    if (!connect_result)
    {
        MEL_TRACE_FATAL("Can't connect to {}", address);
        std::terminate();
    }

    // TODO-JK allow taking values out of result
    zmq::socket_t& socket = connect_result.ok();
    ON_SCOPE_EXIT(socket.disconnect(address));

    char identity[256];
    std::size_t l = 256;
    socket.getsockopt(ZMQ_ROUTING_ID, identity, &l);

    flatbuffers::FlatBufferBuilder const query =
        mel::ncprot::serialize_query(identity, "SELECT * FROM v$sql");

    while (true)
    {
        mel::ncprot::zmq_result<zmq::send_result_t> const send_result =
            mel::ncprot::client::send(socket,
                {reinterpret_cast<std::byte*>(query.GetBufferPointer()),
                    query.GetSize()});

        if (!send_result || !send_result.ok())
        {
            continue;
        }

        mel::ncprot::zmq_result<mel::ncprot::recv_response<zmq::message_t>>
            recv_result = mel::ncprot::client::recv(
                socket); // TODO-JK: This is blocking indefinately

        if (recv_result)
        {
            mel::ncprot::recv_response<zmq::message_t> const& success =
                recv_result.ok();
            if (success.received)
            {
                mel::network::Message const* message =
                    flatbuffers::GetRoot<mel::ncprot::root_type>(
                        success.message.value().data());

                if (message->content_type() ==
                    mel::network::MessageContent_result)
                {
                    mel::network::QueryResult const* query_result =
                        message->content_as_result();
                    MEL_TRACE_INFO("Returned {} rows.", query_result->length());
                }
            }
        }
    }
}
