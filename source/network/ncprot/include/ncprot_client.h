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

namespace mel::ncprot
{
    template<typename T>
    using zmq_result = cppex::result<T, zmq::error_t>;

    template<typename T, typename... Args>
    zmq_result<T> zmq_res_ok(Args&&... args)
    {
        return cppex::res::ok<T, zmq::error_t>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    zmq_result<T> zmq_res_error(Args&&... args)
    {
        return cppex::res::error<T, zmq::error_t>(std::forward<Args>(args)...);
    }

    template<typename T>
    struct recv_response
    {
        zmq::recv_result_t received;
        std::optional<T> message;
    };
} // namespace mel::ncprot

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
