#ifndef MELINDA_NETWORK_NCPROT_NCPROT_TYPES_INCLUDED
#define MELINDA_NETWORK_NCPROT_NCPROT_TYPES_INCLUDED

#include <zmq.hpp>

#include "result.h"

namespace mel::ncprot
{
    template<typename T>
    using zmq_result = cppex::result<T, zmq::error_t>;

    template<typename T>
    struct recv_response
    {
        zmq::recv_result_t received;
        std::optional<T> message;
    };
} // namespace mel::ncprot
#endif
