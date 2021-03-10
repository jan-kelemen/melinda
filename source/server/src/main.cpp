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
        using response_content_t = std::pair<std::string, zmq::message_t>;
        using response_t = std::optional<response_content_t>;
        using recv_t = std::pair<zmq::recv_result_t, response_t>;

        mel::ncprot::server::recv_result_t recv_result =
            mel::ncprot::server::recv(socket);

        if (recv_result.index() != 0)
        {
            continue;
        }
        recv_t const& success = std::get<recv_t>(recv_result);
        if (!success.first)
        {
            continue;
        }

        mel::network::Message const* const message =
            flatbuffers::GetRoot<mel::ncprot::root_type>(
                success.second->second.data());

        if (message->content_type() != mel::network::MessageContent_query)
        {
            continue;
        }

        mel::network::Query const* const query = message->content_as_query();
        MEL_TRACE_INFO("Recevied query {} from {}",
            query->content()->c_str(),
            success.second->first);

        MEL_TRACE_INFO("Sending response to {}", success.second->first);
        mel::ncprot::server::send_result_t send_result =
            mel::ncprot::server::send(socket,
                success.second->first,
                {reinterpret_cast<std::byte*>(query_result.GetBufferPointer()),
                    query_result.GetSize()});

        std::visit(
            [&client = success.second->first](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::same_as<T,
                                  std::variant_alternative_t<0,
                                      decltype(send_result)>>)
                {
                    if (!arg)
                    {
                        MEL_TRACE_ERROR("Can't send response");
                    }
                }
                else if constexpr (std::same_as<T,
                                       std::variant_alternative_t<1,
                                           decltype(send_result)>>)
                {
                    if (arg.num() == EHOSTUNREACH)
                    {
                        MEL_TRACE_WARN(
                            "Client {} has disconnected or is not known.",
                            client);
                    }
                }
                else
                {
                    static_assert(mel::cppex::always_false_v<T>,
                        "non-exhaustive visitor!");
                }
            },
            send_result);
    }
}
