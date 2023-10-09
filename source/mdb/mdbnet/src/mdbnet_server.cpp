#include <mdbnet_server.h>

#include <iterator>
#include <optional>
#include <system_error>
#include <utility>
#include <vector>

#include <tl/expected.hpp>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <mbltrc_trace.h>
#include <mdbnet_types.h>

tl::expected<zmq::socket_t, std::error_code> melinda::mdbnet::server::bind(
    zmq::context_t& context,
    std::string const& address)
{
    zmq::socket_t socket = zmq::socket_t(context, zmq::socket_type::router);

    socket.set(zmq::sockopt::linger, 0); // Close the socket immediately
    socket.set(zmq::sockopt::router_mandatory,
        1); // Report host unreachable errors if the reply can't be routed
    socket.set(zmq::sockopt::sndtimeo,
        10 * 1000); // Set the timeout for sending replies to 10 seconds

    MBLTRC_TRACE_INFO("Registering on '{}' endpoint.", address);
    try
    {
        socket.bind(address);
    }
    catch (zmq::error_t const& ex)
    {
        MBLTRC_TRACE_ERROR("Can't bind to {}. Reason: ZMQ-{}:",
            ex.num(),
            ex.what());
        return tl::unexpected{
            std::make_error_code(static_cast<std::errc>(ex.num()))};
    }
    return std::move(socket);
}

melinda::mdbnet::result<zmq::send_result_t> melinda::mdbnet::server::send(
    zmq::socket_t& socket,
    std::string const& client,
    std::span<std::byte> bytes,
    zmq::send_flags flags)
{
    std::vector<zmq::message_t> response;
    response.emplace_back(client.c_str(), client.size());
    response.emplace_back();
    response.emplace_back(bytes.data(), bytes.size());
    MBLTRC_TRACE_INFO("Sending message of {} bytes to {}.",
        bytes.size(),
        client);
    try
    {
        zmq::send_result_t const result =
            zmq::send_multipart(socket, std::move(response), flags);
        return result;
    }
    catch (zmq::error_t const& ex)
    {
        MBLTRC_TRACE_ERROR(
            "Unexpected error while receiving request. Reason: ZMQ-{}: {}",
            ex.num(),
            ex.what());
        return tl::unexpected{ex};
    }
}

melinda::mdbnet::result<
    melinda::mdbnet::recv_response<melinda::mdbnet::client_message>>
melinda::mdbnet::server::recv(zmq::socket_t& socket, zmq::recv_flags flags)
{
    std::vector<zmq::message_t> messages;
    try
    {
        zmq::recv_result_t const result =
            zmq::recv_multipart(socket, std::back_inserter(messages), flags);
        if (result)
        {
            MBLTRC_TRACE_INFO(
                "Received {} messages of {} bytes in size from {}.",
                messages.size(),
                messages[2].size(),
                messages[0].to_string_view());

            return recv_response<client_message>{.received = result,
                .message = client_message{.identity = messages[0].to_string(),
                    .content = std::move(messages.back())}};
        }

        return recv_response<client_message>{.received = result,
            .message = std::nullopt};
    }
    catch (zmq::error_t const& ex)
    {
        MBLTRC_TRACE_FATAL(
            "Unexpected error while receiving request. Reason: ZMQ-{}: {}",
            ex.num(),
            ex.what());
        return tl::unexpected{ex};
    }
}
