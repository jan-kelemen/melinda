#ifndef MELINDA_MDBNET_SERVER_INCLUDED
#define MELINDA_MDBNET_SERVER_INCLUDED

#include <cstddef>
#include <span>
#include <string>

#include <zmq.hpp>

#include <mblcxx_result.h>
#include <mblcxx_sops.h>

#include <mdbnet_types.h>

namespace melinda::mdbnet
{
    struct client_message final
    {
        std::string identity;
        zmq::message_t content;
    };

    struct server final : mblcxx::sops::none
    {
    public:
        static mblcxx::result<zmq::socket_t> bind(zmq::context_t& context,
            std::string const& address);

        static result<zmq::send_result_t> send(zmq::socket_t& socket,
            std::string const& client,
            std::span<std::byte> bytes,
            zmq::send_flags flags = zmq::send_flags::dontwait);

        static result<recv_response<client_message>> recv(zmq::socket_t& socket,
            zmq::recv_flags flags = zmq::recv_flags::dontwait);
    };

} // namespace melinda::mdbnet

#endif
