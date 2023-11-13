#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <utility>
#include <vector>

#include <boost/numeric/conversion/cast.hpp>
#include <flatbuffers/flatbuffers.h>
#include <tl/expected.hpp>
#include <zmq.hpp>

#include <mblcxx_scope_exit.h>

#include <mbltrc_memory_mapped_sink.h>
#include <mbltrc_trace.h>
#include <mbltrc_trace_level.h>

#include <mdbnet_client.h>
#include <mdbnet_serialization.h>
#include <mdbnet_types.h>
#include <mdbnet_wire_generated.h>

int main()
{
    std::shared_ptr<melinda::mbltrc::memory_mapped_sink> sink{
        std::make_shared<melinda::mbltrc::memory_mapped_sink>(
            melinda::mbltrc::trace_level::debug,
            "../log",
            "mclxmp",
            10 * 1024 * 1024)};

    melinda::mbltrc::register_process_sink(sink);

    zmq::context_t ctx;
    constexpr char const* const address = "tcp://localhost:22365";

    auto connect_result = melinda::mdbnet::client::connect(ctx, address);
    if (!connect_result)
    {
        MBLTRC_TRACE_FATAL("Can't connect to {}", address);
        std::terminate();
    }

    zmq::socket_t socket = std::move(*connect_result);
    MBLCXX_ON_SCOPE_EXIT(socket.disconnect(address));

    std::string const identity = socket.get(zmq::sockopt::routing_id, 256);

    std::vector<std::string> queries{
        "CREATE SCHEMA books;",
        "CREATE SCHEMA books;",
        "SELECT * FROM vsql",
        "CREATE SCHEMA books2",
    };

    for (auto const& query : queries)
    {
        flatbuffers::FlatBufferBuilder const q =
            melinda::mdbnet::serialization::query(identity, query);

        melinda::mdbnet::result<zmq::send_result_t> const send_result =
            melinda::mdbnet::client::send(socket,
                {reinterpret_cast<std::byte*>(q.GetBufferPointer()),
                    q.GetSize()});

        if (!send_result || !*send_result)
        {
            continue;
        }

        melinda::mdbnet::result<melinda::mdbnet::recv_response<zmq::message_t>>
            recv_result = melinda::mdbnet::client::recv(
                socket); // TODO-JK: This is blocking indefinately

        if (recv_result)
        {
            auto success = std::move(*recv_result);
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

                    for (std::size_t i = 0; i != query_result->length(); ++i)
                    {
                        uint32_t const offset = (*query_result->offsets())
                            [boost::numeric_cast<flatbuffers::uoffset_t>(i)];
                        MBLTRC_TRACE_INFO("Row {} value {}",
                            i,
                            (*query_result->raw_values())[offset]);
                    }
                }
            }
        }
    }
}
