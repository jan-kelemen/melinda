#ifndef MELINDA_MBLCXX_RESULT_INCLUDED
#define MELINDA_MBLCXX_RESULT_INCLUDED

#include <tl/expected.hpp>

#include <system_error>

namespace melinda::mblcxx
{
    template<typename Value, typename Error = std::error_code>
    using result = tl::expected<Value, Error>;

    template<typename Value = std::error_code>
    using error = tl::unexpected<Value>;
} // namespace melinda::mblcxx

#endif
