#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

#include <mblcxx_scope_exit.h>

#include <mbltrc_trace.h>

#include <mdbnet_client.h>
#include <mdbnet_serialization.h>

int main()
{
    melinda::mbltrc::trace_options trace_config(std::filesystem::path("../log"),
        std::filesystem::path("example_client"));
    trace_config.level = melinda::mbltrc::trace_level::info;

    melinda::mbltrc::initialize_process_trace_handle(
        melinda::mbltrc::create_trace_handle(trace_config));

    zmq::context_t ctx;
    constexpr char const* const address = "tcp://localhost:22365";

    melinda::mblcxx::result<zmq::socket_t> connect_result =
        melinda::mdbnet::client::connect(ctx, address);
    if (!connect_result)
    {
        MBLTRC_TRACE_FATAL("Can't connect to {}", address);
        std::terminate();
    }

    // TODO-JK allow taking values out of result
    zmq::socket_t& socket = connect_result.ok();
    MBLCXX_ON_SCOPE_EXIT(socket.disconnect(address));

    std::string const identity = socket.get(zmq::sockopt::routing_id, 256);

    flatbuffers::FlatBufferBuilder const query =
        melinda::mdbnet::serialization::query(identity, "SELECT * FROM v$sql");

    while (true)
    {
        melinda::mdbnet::result<zmq::send_result_t> const send_result =
            melinda::mdbnet::client::send(socket,
                {reinterpret_cast<std::byte*>(query.GetBufferPointer()),
                    query.GetSize()});

        if (!send_result || !send_result.ok())
        {
            continue;
        }

        melinda::mdbnet::result<melinda::mdbnet::recv_response<zmq::message_t>>
            recv_result = melinda::mdbnet::client::recv(
                socket); // TODO-JK: This is blocking indefinately

        if (recv_result)
        {
            melinda::mdbnet::recv_response<zmq::message_t> const& success =
                recv_result.ok();
            if (success.received)
            {
                melinda::mdbnet::Message const* message =
                    flatbuffers::GetRoot<melinda::mdbnet::root_type>(
                        success.message.value().data());

                if (message->content_type() ==
                    melinda::mdbnet::MessageContent_result)
                {
                    melinda::mdbnet::QueryResult const* query_result =
                        message->content_as_result();
                    MBLTRC_TRACE_INFO("Returned {} rows.",
                        query_result->length());
                }
            }
        }
    }
}
