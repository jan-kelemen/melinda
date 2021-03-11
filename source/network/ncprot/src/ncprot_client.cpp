#include "ncprot_client.h"

#include <array>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <system_error>

#include "trace.h"
#include "wire_generated.h"

namespace
{
    // Generates the identity of the client which is used to register on the
    // server side.
    mel::cppex::result<std::string> generate_identity()
    {
        constexpr std::size_t const max_identity_length = 255;

        boost::uuids::random_generator generator;
        std::string const uuid = to_string(generator());

        std::array<char, HOST_NAME_MAX + 1> buffer = {};
        if (gethostname(buffer.data(), HOST_NAME_MAX) == -1)
        {
            return mel::cppex::res::error<std::string>(
                std::make_error_code(static_cast<std::errc>(errno)));
        }

        std::string rv = uuid + "_" + buffer.data();
        if (rv.length() > max_identity_length)
        {
            rv.resize(max_identity_length);
        }

        return mel::cppex::res::ok<std::string>(rv);
    }
} // namespace

flatbuffers::FlatBufferBuilder mel::ncprot::serialize_query(
    std::string const& identity,
    std::string const& query)
{
    flatbuffers::FlatBufferBuilder raw_builder;

    auto const client_off = raw_builder.CreateString(identity);

    auto const query_off =
        network::CreateQueryDirect(raw_builder, query.c_str());

    network::MessageBuilder message_builder(raw_builder);

    message_builder.add_content_type(mel::network::MessageContent_query);
    message_builder.add_content(query_off.Union());
    message_builder.add_client(client_off);

    raw_builder.Finish(message_builder.Finish());

    return raw_builder;
}

flatbuffers::FlatBufferBuilder mel::ncprot::serialize_query_result(
    std::string const& identity,
    std::uint32_t length,
    std::span<uint32_t> const& offsets,
    std::span<std::byte> const& data)
{
    flatbuffers::FlatBufferBuilder raw_builder;

    auto const client_off = raw_builder.CreateString(identity);
    auto const offsets_off =
        raw_builder.CreateVector(offsets.data(), offsets.size());
    static_assert(sizeof(uint8_t) == sizeof(std::byte),
        "flatbuffers use uint8_t as a byte representation");
    auto const raw_values_off =
        raw_builder.CreateVector(reinterpret_cast<uint8_t const*>(data.data()),
            data.size());

    network::QueryResultBuilder query_result_builder(raw_builder);

    query_result_builder.add_length(length);
    query_result_builder.add_offsets(offsets_off);
    query_result_builder.add_raw_values(raw_values_off);

    auto const query_off = query_result_builder.Finish();

    network::MessageBuilder message_builder(raw_builder);

    message_builder.add_content_type(mel::network::MessageContent_result);
    message_builder.add_content(query_off.Union());
    message_builder.add_client(client_off);

    raw_builder.Finish(message_builder.Finish());

    return raw_builder;
}

mel::cppex::result<zmq::socket_t> mel::ncprot::client::connect(
    zmq::context_t& context,
    std::string const& address)
{
    cppex::result<std::string> const identity = generate_identity();

    if (!identity)
    {
        MEL_TRACE_ERROR("Unable to create client identity: {}!",
            identity.error().message());
        return cppex::res::error<zmq::socket_t>(identity.error());
    }
    std::string const& raw_identity = identity.ok();

    zmq::socket_t socket = zmq::socket_t(context, zmq::socket_type::req);

    socket.setsockopt(ZMQ_LINGER, 0); // Close the socket immediately
    socket.setsockopt(ZMQ_SNDTIMEO,
        10 * 1000); // Set the timeout for sending requests to 10 seconds
    socket.setsockopt(ZMQ_ROUTING_ID,
        raw_identity.c_str(),
        raw_identity.length()); // Identify to the server with a unique name

    MEL_TRACE_INFO("Connecting to {}.", address);
    try
    {
        socket.connect(address.c_str());
    }
    catch (zmq::error_t const& ex)
    {
        MEL_TRACE_ERROR("Can't connect to {}. Reason: ZMQ-{}: {}",
            address,
            ex.num(),
            ex.what());
        return cppex::res::error<zmq::socket_t>(
            std::make_error_code(static_cast<std::errc>(errno)));
    }
    return cppex::res::ok<zmq::socket_t>(std::move(socket));
}

mel::ncprot::zmq_result<zmq::send_result_t> mel::ncprot::client::send(
    zmq::socket_t& socket,
    std::span<std::byte> bytes,
    zmq::send_flags flags)
{
    zmq::message_t message = zmq::message_t(bytes.data(), bytes.size());
    MEL_TRACE_INFO("Sending message of {} bytes.", bytes.size());
    try
    {
        zmq::send_result_t const result = socket.send(message, flags);
        if (!result)
        {
            MEL_TRACE_ERROR("Can't send message, peer might be unreachable");
        }
        return zmq_res_ok<zmq::send_result_t>(result);
    }
    catch (zmq::error_t const& ex)
    {
        MEL_TRACE_ERROR(
            "Unexpected error while sending message. Reason: ZMQ-{}: {}",
            ex.num(),
            ex.what());
        return zmq_res_error<zmq::send_result_t>(ex);
    }
}

mel::ncprot::zmq_result<mel::ncprot::recv_response<zmq::message_t>>
mel::ncprot::client::recv(zmq::socket_t& socket, zmq::recv_flags flags)
{
    zmq::message_t message;
    try
    {
        zmq::recv_result_t const result = socket.recv(message, flags);
        MEL_TRACE_INFO("Received message of {} bytes", result.value());
        // clang-format off
        return zmq_res_ok<recv_response<zmq::message_t>>(
            recv_response<zmq::message_t> 
            {
                .received = result,
                .message = std::optional<zmq::message_t>(std::move(message))
            });
        // clang-format on
    }
    catch (zmq::error_t const& ex)
    {
        MEL_TRACE_ERROR(
            "Unexpected error while receiving message. Reason: ZMQ-{}: {}",
            ex.num(),
            ex.what());
        return zmq_res_error<recv_response<zmq::message_t>>(ex);
    }
}

mel::cppex::result<zmq::socket_t>
mel::ncprot::server::bind(zmq::context_t& context, std::string const& address)
{
    zmq::socket_t socket = zmq::socket_t(context, zmq::socket_type::router);

    socket.setsockopt(ZMQ_LINGER, 0); // Close the socket immediately
    socket.setsockopt(ZMQ_ROUTER_MANDATORY,
        1); // Report host unreachable errors if the reply can't be routed
    socket.setsockopt(ZMQ_SNDTIMEO,
        10 * 1000); // Set the timeout for sending replies to 10 seconds

    MEL_TRACE_INFO("Registering on '{}' endpoint.", address);
    try
    {
        socket.bind(address);
    }
    catch (zmq::error_t const& ex)
    {
        MEL_TRACE_ERROR("Can't bind to {}. Reason: ZMQ-{}:",
            ex.num(),
            ex.what());
        return cppex::res::error<zmq::socket_t>(
            std::make_error_code(static_cast<std::errc>(ex.num())));
    }
    return cppex::res::ok<zmq::socket_t>(std::move(socket));
}

mel::ncprot::zmq_result<zmq::send_result_t> mel::ncprot::server::send(
    zmq::socket_t& socket,
    std::string const& client,
    std::span<std::byte> bytes,
    zmq::send_flags flags)
{
    std::vector<zmq::message_t> response;
    response.emplace_back(client.c_str(), client.size());
    response.emplace_back();
    response.emplace_back(bytes.data(), bytes.size());
    MEL_TRACE_INFO("Sending message of {} bytes to {}.", bytes.size(), client);
    try
    {
        zmq::send_result_t const result =
            zmq::send_multipart(socket, std::move(response), flags);
        return zmq_res_ok<zmq::send_result_t>(result);
    }
    catch (zmq::error_t const& ex)
    {
        MEL_TRACE_ERROR(
            "Unexpected error while receiving request. Reason: ZMQ-{}: {}",
            ex.num(),
            ex.what());
        return zmq_res_error<zmq::send_result_t>(ex);
    }
}

mel::ncprot::zmq_result<
    mel::ncprot::recv_response<mel::ncprot::server::client_message>>
mel::ncprot::server::recv(zmq::socket_t& socket, zmq::recv_flags flags)
{
    std::vector<zmq::message_t> messages;
    try
    {
        zmq::recv_result_t const result =
            zmq::recv_multipart(socket, std::back_inserter(messages), flags);
        if (result)
        {
            MEL_TRACE_INFO("Received {} messages of {} bytes in size from {}.",
                messages.size(),
                messages[2].size(),
                messages[0].to_string_view());

            // clang-format off
            return zmq_res_ok<recv_response<client_message>>(
                recv_response<mel::ncprot::server::client_message> 
                {
                    .received = result,
                    .message = mel::ncprot::server::client_message 
                    {
                        .identity = messages[0].to_string(),
                        .content = std::move(messages.back())
                    }
                });
            // clang-fromat on
        }

        // clang-format off
        return zmq_res_ok<recv_response<client_message>>(
            recv_response<mel::ncprot::server::client_message> 
            {
                .received = result,
                .message = std::nullopt
            });
        // clang-format on
    }
    catch (zmq::error_t const& ex)
    {
        MEL_TRACE_FATAL(
            "Unexpected error while receiving request. Reason: ZMQ-{}: {}",
            ex.num(),
            ex.what());
        return zmq_res_error<recv_response<client_message>>(ex);
    }
}
