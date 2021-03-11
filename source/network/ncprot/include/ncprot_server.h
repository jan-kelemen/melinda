#ifndef MELINDA_NETWORK_NCPROT_NCPROT_SERVER_INCLUDED
#define MELINDA_NETWORK_NCPROT_NCPROT_SERVER_INCLUDED

#include <cstddef>
#include <span>
#include <string>

#include <zmq.hpp>

#include "result.h"

#include "ncprot_types.h"

namespace mel::ncprot::server
{
    struct client_message
    {
        std::string identity;
        zmq::message_t content;
    };

    cppex::result<zmq::socket_t> bind(zmq::context_t& context,
        std::string const& address);

    zmq_result<zmq::send_result_t> send(zmq::socket_t& socket,
        std::string const& client,
        std::span<std::byte> bytes,
        zmq::send_flags flags = zmq::send_flags::dontwait);

    zmq_result<recv_response<client_message>> recv(zmq::socket_t& socket,
        zmq::recv_flags flags = zmq::recv_flags::dontwait);
} // namespace mel::ncprot::server

#endif
