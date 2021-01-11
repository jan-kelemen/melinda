#ifndef MELINDA_NETWORK_NCPROT_NCPROT_INTERNAL_INCLUDED
#define MELINDA_NETWORK_NCPROT_NCPROT_INTERNAL_INCLUDED

#include <zmq.hpp>

namespace mel::ncprot
{
    zmq::context_t& context();
}

#endif
