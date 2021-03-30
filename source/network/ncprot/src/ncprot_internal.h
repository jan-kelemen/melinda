#ifndef MELINDA_NETWORK_NCPROT_NCPROT_INTERNAL_INCLUDED
#define MELINDA_NETWORK_NCPROT_NCPROT_INTERNAL_INCLUDED

#include <zmq.hpp>

#include "ncprot_types.h"

namespace mel::ncprot
{
    zmq::context_t& context();

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
} // namespace mel::ncprot

#endif
