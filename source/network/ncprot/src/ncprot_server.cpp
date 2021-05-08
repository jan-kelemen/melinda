#include "ncprot_server.h"

#include <system_error>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "trace.h"

#include "ncprot_internal.h"

mel::cppex::result<zmq::socket_t>
mel::ncprot::server::bind(zmq::context_t& context, std::string const& address)
{
    zmq::socket_t socket = zmq::socket_t(context, zmq::socket_type::router);

    socket.set(zmq::sockopt::linger, 0); // Close the socket immediately
    socket.set(zmq::sockopt::router_mandatory,
        1); // Report host unreachable errors if the reply can't be routed
    socket.set(zmq::sockopt::sndtimeo,
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
