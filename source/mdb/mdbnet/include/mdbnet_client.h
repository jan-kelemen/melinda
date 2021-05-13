#ifndef MELINDA_MDBNET_CLIENT_INCLUDED
#define MELINDA_MDBNET_CLIENT_INCLUDED

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <variant>

#include <zmq.hpp>

#include <mblcxx_result.h>

#include <mdbnet_types.h>

namespace melinda::mdbnet
{
    struct client final
    {
    public:
        static mblcxx::result<zmq::socket_t> connect(zmq::context_t& context,
            std::string const& address);

        static result<zmq::send_result_t> send(zmq::socket_t& socket,
            std::span<std::byte> bytes,
            zmq::send_flags flags = zmq::send_flags::none);

        static result<recv_response<zmq::message_t>> recv(zmq::socket_t& socket,
            zmq::recv_flags flags = zmq::recv_flags::none);

    public:
        client() = delete;

        client(client const&) = delete;

        client(client&&) noexcept = delete;

    public:
        ~client() = delete;

    public:
        client& operator=(client const&) = delete;

        client& operator=(client&&) noexcept = delete;
    };
} // namespace melinda::mdbnet

#endif
