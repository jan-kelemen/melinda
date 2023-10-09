#ifndef MELINDA_MDBNET_CLIENT_INCLUDED
#define MELINDA_MDBNET_CLIENT_INCLUDED

#include <cstddef>
#include <span>
#include <string>
#include <system_error>

#include <tl/expected.hpp>
#include <zmq.hpp>

#include <mdbnet_types.h>

namespace melinda::mdbnet
{
    struct client final
    {
    public:
        static tl::expected<zmq::socket_t, std::error_code>
        connect(zmq::context_t& context, std::string const& address);

        static result<zmq::send_result_t> send(zmq::socket_t& socket,
            std::span<std::byte> bytes,
            zmq::send_flags flags = zmq::send_flags::none);

        static result<recv_response<zmq::message_t>> recv(zmq::socket_t& socket,
            zmq::recv_flags flags = zmq::recv_flags::none);

    public:
        client() = delete;

        ~client() noexcept = delete;
    };
} // namespace melinda::mdbnet

#endif
