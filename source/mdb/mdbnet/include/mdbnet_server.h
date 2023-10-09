#ifndef MELINDA_MDBNET_SERVER_INCLUDED
#define MELINDA_MDBNET_SERVER_INCLUDED

#include <cstddef>
#include <span>
#include <string>
#include <system_error>

#include <tl/expected.hpp>
#include <zmq.hpp>

#include <mdbnet_types.h>

namespace melinda::mdbnet
{
    struct client_message final
    {
        std::string identity;
        zmq::message_t content;
    };

    struct server final
    {
    public:
        static tl::expected<zmq::socket_t, std::error_code>
        bind(zmq::context_t& context, std::string const& address);

        static result<zmq::send_result_t> send(zmq::socket_t& socket,
            std::string const& client,
            std::span<std::byte> bytes,
            zmq::send_flags flags = zmq::send_flags::dontwait);

        static result<recv_response<client_message>> recv(zmq::socket_t& socket,
            zmq::recv_flags flags = zmq::recv_flags::dontwait);

    public:
        server() = delete;

        ~server() noexcept = delete;
    };

} // namespace melinda::mdbnet

#endif
