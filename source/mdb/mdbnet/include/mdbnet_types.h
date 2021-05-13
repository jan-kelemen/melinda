#ifndef MELINDA_MDBNET_TYPES_INCLUDED
#define MELINDA_MDBNET_TYPES_INCLUDED

#include <optional>

#include <zmq.hpp>

#include <mblcxx_result.h>

namespace melinda::mdbnet
{
    template<typename T>
    using result = mblcxx::result<T, zmq::error_t>;

    template<typename T, typename E = zmq::error_t>
    using res = mblcxx::res<T, E>;

    template<typename T>
    struct recv_response final
    {
        zmq::recv_result_t received;
        std::optional<T> message;
    };
} // namespace melinda::mdbnet
#endif
