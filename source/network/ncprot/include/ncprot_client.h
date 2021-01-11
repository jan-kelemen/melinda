#ifndef MELINDA_NETWORK_NCPROT_NCPROT_CLIENT_INCLUDED
#define MELINDA_NETWORK_NCPROT_NCPROT_CLIENT_INCLUDED

#include <string>

#include "result.h"

namespace mel::ncprot
{
    // Generates the identity of the client which is used to register
    // on the server side.
    mel::cppex::result<std::string> generate_identity();
} // namespace mel::ncprot

#endif
