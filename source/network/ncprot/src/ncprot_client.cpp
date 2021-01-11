#include "ncprot_client.h"

#include <array>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

mel::cppex::result<std::string> mel::ncprot::generate_identity()
{
    constexpr std::size_t const max_identity_length = 255;

    boost::uuids::random_generator generator;
    std::string const uuid = to_string(generator());

    std::array<char, HOST_NAME_MAX + 1> buffer = {};
    if (gethostname(buffer.data(), HOST_NAME_MAX) == -1)
    {
        return mel::cppex::result<std::string>(
            std::make_error_code(static_cast<std::errc>(errno)));
    }

    std::string rv = uuid + "_" + buffer.data();
    if (rv.length() > max_identity_length)
    {
        rv.resize(max_identity_length);
    }

    return mel::cppex::result<std::string>(rv);
}
