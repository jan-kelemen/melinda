#include <mdbnet_internal.h>

#include <zmq.hpp>

zmq::context_t& melinda::mdbnet::context()
{
    static zmq::context_t ctx;
    return ctx;
}
