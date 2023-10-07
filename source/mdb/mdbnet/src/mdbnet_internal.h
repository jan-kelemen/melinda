#ifndef MELINDA_MDBNET_INTERNAL_INCLUDED
#define MELINDA_MDBNET_INTERNAL_INCLUDED

namespace zmq
{
    class context_t;
} // namespace zmq

namespace melinda::mdbnet
{
    zmq::context_t& context();
} // namespace melinda::mdbnet

#endif
