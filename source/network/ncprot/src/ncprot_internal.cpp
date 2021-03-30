#include "ncprot_internal.h"

zmq::context_t& mel::ncprot::context()
{
    static zmq::context_t ctx;
    return ctx;
}
