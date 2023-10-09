#include <array>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <memory>
#include <optional>
#include <signal.h>
#include <system_error>
#include <utility>

#include <flatbuffers/flatbuffers.h>

#include <tl/expected.hpp>

#include <zmq.hpp>

#include <mblcxx_scope_exit.h>

#include <mbltrc_memory_mapped_sink.h>
#include <mbltrc_trace.h>
#include <mbltrc_trace_level.h>

#include <mdbsql_engine.h>

#include <mdbnet_serialization.h>
#include <mdbnet_server.h>
#include <mdbnet_types.h>
#include <mdbnet_wire_generated.h>

#include <mdbsrv_options.h>

bool run{true};

void sighandler(int sig)
{
    MBLTRC_TRACE_INFO("Signal {} caught. Stopping execution", sig);
    run = false;
}

int main(int argc, char** argv)
{
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGINT, &sighandler);

    std::optional<melinda::mdbsrv::options> const options{
        melinda::mdbsrv::parse_options(argc, argv)};
    if (!options)
    {
        return EXIT_FAILURE;
    }

    std::shared_ptr<melinda::mbltrc::memory_mapped_sink> sink{
        std::make_shared<melinda::mbltrc::memory_mapped_sink>(
            melinda::mbltrc::trace_level::debug,
            options->data_directory,
            "mdbsrv",
            10 * 1024 * 1024)};
    melinda::mbltrc::register_process_sink(sink);

    melinda::mdbsql::engine engine{options->data_directory};

    zmq::context_t ctx;
    constexpr char const* const address = "tcp://*:22365";

    auto bind_result = melinda::mdbnet::server::bind(ctx, address);
    if (!bind_result)
    {
        MBLTRC_TRACE_FATAL("Can't connect to {}", address);
        std::terminate();
    }

    zmq::socket_t socket = std::move(*bind_result);
    MBLCXX_ON_SCOPE_EXIT(socket.unbind(address));

    while (run)
    {
        auto recv_result = melinda::mdbnet::server::recv(socket);

        if (!recv_result || !recv_result->received)
        {
            continue;
        }

        auto const& success = *recv_result;

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

        bool const result = engine.execute(query->content()->c_str());

        std::array<uint32_t, 1> offsets{0};
        std::array<std::byte, 1> raw_values{std::byte{result}};
        flatbuffers::FlatBufferBuilder query_result =
            melinda::mdbnet::serialization::query_result(address,
                1,
                offsets,
                raw_values);

        MBLTRC_TRACE_INFO("Sending response to {}", success.message->identity);
        melinda::mdbnet::result<zmq::send_result_t> const send_result =
            melinda::mdbnet::server::send(socket,
                success.message->identity,
                {reinterpret_cast<std::byte*>(query_result.GetBufferPointer()),
                    query_result.GetSize()});

        if (send_result)
        {
            if (!send_result.value())
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
