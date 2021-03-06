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
#include <mblcxx_sops.h>

#include <mdbnet_types.h>

namespace melinda::mdbnet
{
    struct client final : public mblcxx::sops::none
    {
    public:
        static mblcxx::result<zmq::socket_t> connect(zmq::context_t& context,
            std::string const& address);

        static result<zmq::send_result_t> send(zmq::socket_t& socket,
            std::span<std::byte> bytes,
            zmq::send_flags flags = zmq::send_flags::none);

        static result<recv_response<zmq::message_t>> recv(zmq::socket_t& socket,
            zmq::recv_flags flags = zmq::recv_flags::none);
    };
} // namespace melinda::mdbnet

#endif
