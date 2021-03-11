#include <algorithm>
#include <array>
#include <flatbuffers/flatbuffers.h>
#include <iostream>
#include <vector>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "ncprot_client.h"
#include "scope_exit.h"
#include "trace.h"
#include "wire_generated.h"

int main()
{
    mel::trace::trace_options trace_config(fs::path("../log"),
        fs::path("server"));
    trace_config.level = mel::trace::trace_level::info;

    mel::trace::initialize_process_trace_handle(
        mel::trace::create_trace_handle(trace_config));

    zmq::context_t ctx;
    constexpr char const* const address = "tcp://*:22365";

    mel::cppex::result<zmq::socket_t> bind_result =
        mel::ncprot::server::bind(ctx, address);
    if (!bind_result)
    {
        MEL_TRACE_FATAL("Can't connect to {}", address);
        std::terminate();
    }

    zmq::socket_t& socket = bind_result.ok();
    ON_SCOPE_EXIT(socket.unbind(address));

    flatbuffers::FlatBufferBuilder query_result =
        mel::ncprot::serialize_query_result(address, 0, {}, {});

    while (true)
    {
        mel::ncprot::zmq_result<mel::ncprot::recv_response<
            mel::ncprot::server::client_message>> const recv_result =
            mel::ncprot::server::recv(socket);

        if (!recv_result || !recv_result.ok().received)
        {
            continue;
        }

        mel::ncprot::recv_response<mel::ncprot::server::client_message> const&
            success = recv_result.ok();

        mel::network::Message const* const message =
            flatbuffers::GetRoot<mel::ncprot::root_type>(
                success.message->content.data());

        if (message->content_type() != mel::network::MessageContent_query)
        {
            continue;
        }

        mel::network::Query const* const query = message->content_as_query();
        MEL_TRACE_INFO("Recevied query {} from {}",
            query->content()->c_str(),
            success.message->identity);

        MEL_TRACE_INFO("Sending response to {}", success.message->identity);
        mel::ncprot::zmq_result<zmq::send_result_t> const send_result =
            mel::ncprot::server::send(socket,
                success.message->identity,
                {reinterpret_cast<std::byte*>(query_result.GetBufferPointer()),
                    query_result.GetSize()});

        if (send_result)
        {
            if (!send_result.ok())
            {
                MEL_TRACE_ERROR("Can't send response to {}",
                    success.message->identity);
            }
            continue;
        }

        if (send_result.error().num() == EHOSTUNREACH)
        {
            MEL_TRACE_ERROR("Client {} has disconnected or is not known",
                success.message->identity);
            continue;
        }
    }
}
