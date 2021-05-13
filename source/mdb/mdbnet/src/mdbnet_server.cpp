#include <mdbnet_server.h>

#include <system_error>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <mbltrc_trace.h>

#include "mdbnet_internal.h"

namespace melinda::mdbnet
{
    mblcxx::result<zmq::socket_t> server::bind(zmq::context_t& context,
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
            return mblcxx::res<zmq::socket_t>::error(
                std::make_error_code(static_cast<std::errc>(ex.num())));
        }
        return mblcxx::res<zmq::socket_t>::ok(std::move(socket));
    }

    result<zmq::send_result_t> server::send(zmq::socket_t& socket,
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
            return res<zmq::send_result_t>::ok(result);
        }
        catch (zmq::error_t const& ex)
        {
            MBLTRC_TRACE_ERROR(
                "Unexpected error while receiving request. Reason: ZMQ-{}: {}",
                ex.num(),
                ex.what());
            return res<zmq::send_result_t>::error(ex);
        }
    }

    result<recv_response<client_message>> server::recv(zmq::socket_t& socket,
        zmq::recv_flags flags)
    {
        std::vector<zmq::message_t> messages;
        try
        {
            zmq::recv_result_t const result = zmq::recv_multipart(socket,
                std::back_inserter(messages),
                flags);
            if (result)
            {
                MBLTRC_TRACE_INFO(
                    "Received {} messages of {} bytes in size from {}.",
                    messages.size(),
                    messages[2].size(),
                    messages[0].to_string_view());

                // clang-format off
            return res<recv_response<client_message>>::ok(
                recv_response<client_message> 
                {
                    .received = result,
                    .message = client_message 
                    {
                        .identity = messages[0].to_string(),
                        .content = std::move(messages.back())
                    }
                });
            // clang-fromat on
        }

        // clang-format off
        return res<recv_response<client_message>>::ok(
            recv_response<client_message> 
            {
                .received = result,
                .message = std::nullopt
            });
            // clang-format on
        }
        catch (zmq::error_t const& ex)
        {
            MBLTRC_TRACE_FATAL(
                "Unexpected error while receiving request. Reason: ZMQ-{}: {}",
                ex.num(),
                ex.what());
            return res<recv_response<client_message>>::error(ex);
        }
    }
} // namespace melinda::mdbnet
