#include "mdbnet_internal.h"

zmq::context_t& melinda::mdbnet::context()
{
    static zmq::context_t ctx;
    return ctx;
}
