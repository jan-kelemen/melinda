#ifndef MELINDA_NETWORK_NCPROT_NCPROT_CLIENT_INCLUDED
#define MELINDA_NETWORK_NCPROT_NCPROT_CLIENT_INCLUDED

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <variant>

#include <flatbuffers/flatbuffers.h>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "result.h"
#include "wire_generated.h"

namespace mel::ncprot
{
    using root_type = mel::network::Message;

    flatbuffers::FlatBufferBuilder serialize_query(std::string const& identity,
        std::string const& query);

    flatbuffers::FlatBufferBuilder serialize_query_result(
        std::string const& identity,
        std::uint32_t length,
        std::span<uint32_t> const& offsets,
        std::span<std::byte> const& data);
} // namespace mel::ncprot

namespace mel::ncprot::client
{
    using send_result_t = std::variant<zmq::send_result_t, zmq::error_t>;
    using recv_result_t =
        std::variant<std::pair<zmq::recv_result_t, zmq::message_t>,
            zmq::error_t>;

    cppex::result<zmq::socket_t> connect(zmq::context_t& context,
        std::string const& address);

    send_result_t send(zmq::socket_t& socket,
        std::span<std::byte> bytes,
        zmq::send_flags flags = zmq::send_flags::none);

    recv_result_t recv(zmq::socket_t& socket,
        zmq::recv_flags flags = zmq::recv_flags::none);

} // namespace mel::ncprot::client

namespace mel::ncprot::server
{
    using send_result_t = std::variant<zmq::send_result_t, zmq::error_t>;

    // TODO-JK: Clean up this horrible return type
    using recv_result_t =
        std::variant<std::pair<zmq::recv_result_t,
                         std::optional<std::pair<std::string, zmq::message_t>>>,
            zmq::error_t>;

    cppex::result<zmq::socket_t> bind(zmq::context_t& context,
        std::string const& address);

    send_result_t send(zmq::socket_t& socket,
        std::string const& client,
        std::span<std::byte> bytes,
        zmq::send_flags flags = zmq::send_flags::dontwait);

    recv_result_t recv(zmq::socket_t& socket,
        zmq::recv_flags flags = zmq::recv_flags::dontwait);
} // namespace mel::ncprot::server

#endif
