#include <mdbnet_client.h>

#include <array>
#include <cerrno>
#include <climits>
#include <optional>
#include <system_error>
#include <utility>

#include <unistd.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <mbltrc_trace.h>
#include <mdbnet_types.h>

namespace
{
    // Generates the identity of the client which is used to register on the
    // server side.
    tl::expected<std::string, std::error_code> generate_identity()
    {
        constexpr std::size_t const max_identity_length = 255;

        boost::uuids::random_generator generator;
        std::string const uuid = to_string(generator());

        std::array<char, HOST_NAME_MAX + 1> buffer = {};
        if (gethostname(buffer.data(), HOST_NAME_MAX) == -1)
        {
            return tl::unexpected{
                std::make_error_code(static_cast<std::errc>(errno))};
        }

        std::string rv = uuid + "_" + buffer.data();
        if (rv.length() > max_identity_length)
        {
            rv.resize(max_identity_length);
        }

        return rv;
    }
} // namespace

tl::expected<zmq::socket_t, std::error_code> melinda::mdbnet::client::connect(
    zmq::context_t& context,
    std::string const& address)
{
    tl::expected<std::string, std::error_code> const identity =
        generate_identity();

    if (!identity)
    {
        MBLTRC_TRACE_ERROR("Unable to create client identity: {}!",
            identity.error().message());
        return tl::unexpected{identity.error()};
    }
    std::string const& raw_identity = *identity;

    zmq::socket_t socket = zmq::socket_t(context, zmq::socket_type::req);

    socket.set(zmq::sockopt::linger, 0); // Close the socket immediately
    socket.set(zmq::sockopt::sndtimeo,
        10 * 1000); // Set the timeout for sending requests to 10 seconds
    socket.set(zmq::sockopt::routing_id,
        raw_identity); // Identify to the server with a unique name

    MBLTRC_TRACE_INFO("Connecting to {}.", address);
    try
    {
        socket.connect(address.c_str());
    }
    catch (zmq::error_t const& ex)
    {
        MBLTRC_TRACE_ERROR("Can't connect to {}. Reason: ZMQ-{}: {}",
            address,
            ex.num(),
            ex.what());
        return tl::unexpected{
            std::make_error_code(static_cast<std::errc>(errno))};
    }
    return std::move(socket);
}

melinda::mdbnet::result<zmq::send_result_t> melinda::mdbnet::client::send(
    zmq::socket_t& socket,
    std::span<std::byte> bytes,
    zmq::send_flags flags)
{
    zmq::message_t message = zmq::message_t(bytes.data(), bytes.size());
    MBLTRC_TRACE_INFO("Sending message of {} bytes.", bytes.size());
    try
    {
        zmq::send_result_t const result = socket.send(message, flags);
        if (!result)
        {
            MBLTRC_TRACE_ERROR("Can't send message, peer might be unreachable");
        }
        return result;
    }
    catch (zmq::error_t const& ex)
    {
        MBLTRC_TRACE_ERROR(
            "Unexpected error while sending message. Reason: ZMQ-{}: {}",
            ex.num(),
            ex.what());
        return tl::unexpected{ex};
    }
}

melinda::mdbnet::result<melinda::mdbnet::recv_response<zmq::message_t>>
melinda::mdbnet::client::recv(zmq::socket_t& socket, zmq::recv_flags flags)
{
    zmq::message_t message;
    try
    {
        zmq::recv_result_t const result = socket.recv(message, flags);
        MBLTRC_TRACE_INFO("Received message of {} bytes", result.value());
        return recv_response<zmq::message_t>{.received = result,
            .message = std::optional<zmq::message_t>(std::move(message))};
    }
    catch (zmq::error_t const& ex)
    {
        MBLTRC_TRACE_ERROR(
            "Unexpected error while receiving message. Reason: ZMQ-{}: {}",
            ex.num(),
            ex.what());
        return tl::unexpected{ex};
    }
}
