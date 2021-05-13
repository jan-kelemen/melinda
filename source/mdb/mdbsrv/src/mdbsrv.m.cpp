#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

#include <mblcxx_scope_exit.h>

#include <mbltrc_trace.h>

#include <mdbnet_serialization.h>
#include <mdbnet_server.h>

int main()
{
    melinda::mbltrc::trace_options trace_config(std::filesystem::path("../log"),
        std::filesystem::path("server"));
    trace_config.level = melinda::mbltrc::trace_level::info;

    melinda::mbltrc::initialize_process_trace_handle(
        melinda::mbltrc::create_trace_handle(trace_config));

    zmq::context_t ctx;
    constexpr char const* const address = "tcp://*:22365";

    melinda::mblcxx::result<zmq::socket_t> bind_result =
        melinda::mdbnet::server::bind(ctx, address);
    if (!bind_result)
    {
        MBLTRC_TRACE_FATAL("Can't connect to {}", address);
        std::terminate();
    }

    zmq::socket_t& socket = bind_result.ok();
    MBLCXX_ON_SCOPE_EXIT(socket.unbind(address));

    flatbuffers::FlatBufferBuilder query_result =
        melinda::mdbnet::serialization::query_result(address, 0, {}, {});

    while (true)
    {
        melinda::mdbnet::result<melinda::mdbnet::recv_response<
            melinda::mdbnet::client_message>> const recv_result =
            melinda::mdbnet::server::recv(socket);

        if (!recv_result || !recv_result.ok().received)
        {
            continue;
        }

        melinda::mdbnet::recv_response<melinda::mdbnet::client_message> const&
            success = recv_result.ok();

        melinda::mdbnet::Message const* const message =
            flatbuffers::GetRoot<melinda::mdbnet::root_type>(
                success.message->content.data());

        if (message->content_type() != melinda::mdbnet::MessageContent_query)
        {
            continue;
        }

        melinda::mdbnet::Query const* const query = message->content_as_query();
        MBLTRC_TRACE_INFO("Recevied query {} from {}",
            query->content()->c_str(),
            success.message->identity);

        MBLTRC_TRACE_INFO("Sending response to {}", success.message->identity);
        melinda::mdbnet::result<zmq::send_result_t> const send_result =
            melinda::mdbnet::server::send(socket,
                success.message->identity,
                {reinterpret_cast<std::byte*>(query_result.GetBufferPointer()),
                    query_result.GetSize()});

        if (send_result)
        {
            if (!send_result.ok())
            {
                MBLTRC_TRACE_ERROR("Can't send response to {}",
                    success.message->identity);
            }
            continue;
        }

        if (send_result.error().num() == EHOSTUNREACH)
        {
            MBLTRC_TRACE_ERROR("Client {} has disconnected or is not known",
                success.message->identity);
            continue;
        }
    }
}
