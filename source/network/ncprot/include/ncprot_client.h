#ifndef MELINDA_NETWORK_NCPROT_NCPROT_CLIENT_INCLUDED
#define MELINDA_NETWORK_NCPROT_NCPROT_CLIENT_INCLUDED

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <variant>

#include <zmq.hpp>

#include "result.h"

#include "ncprot_types.h"

namespace mel::ncprot::client
{
    cppex::result<zmq::socket_t> connect(zmq::context_t& context,
        std::string const& address);

    zmq_result<zmq::send_result_t> send(zmq::socket_t& socket,
        std::span<std::byte> bytes,
        zmq::send_flags flags = zmq::send_flags::none);

    zmq_result<recv_response<zmq::message_t>> recv(zmq::socket_t& socket,
        zmq::recv_flags flags = zmq::recv_flags::none);

} // namespace mel::ncprot::client

#endif
