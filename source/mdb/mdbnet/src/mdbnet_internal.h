#ifndef MELINDA_NETWORK_NCPROT_NCPROT_INTERNAL_INCLUDED
#define MELINDA_NETWORK_NCPROT_NCPROT_INTERNAL_INCLUDED

#include <zmq.hpp>

#include <mdbnet_types.h>

namespace melinda::mdbnet
{
zmq::context_t& context();
} // namespace melinda::mdbnet

#endif
