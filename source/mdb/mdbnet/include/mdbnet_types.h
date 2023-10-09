#ifndef MELINDA_MDBNET_TYPES_INCLUDED
#define MELINDA_MDBNET_TYPES_INCLUDED

#include <optional>

#include <zmq.hpp>

#include <tl/expected.hpp>

namespace melinda::mdbnet
{
    template<typename T>
    using result = tl::expected<T, zmq::error_t>;

    template<typename T>
    struct recv_response final
    {
        zmq::recv_result_t received;
        std::optional<T> message;
    };
} // namespace melinda::mdbnet
#endif
