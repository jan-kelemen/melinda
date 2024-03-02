#ifndef MELINDA_MBLCXX_EXPECTED_INCLUDED
#define MELINDA_MBLCXX_EXPECTED_INCLUDED

#ifdef MELINDA_MBLCXX_USE_TL_EXPECTED
#include <tl/expected.hpp> // IWYU pragma: export
#else
#include <expected> // IWYU pragma: export
#endif

#include <system_error> // IWYU pragma: export

namespace melinda::mblcxx
{
#ifdef MELINDA_MBLCXX_USE_TL_EXPECTED
    using tl::expected;
    using tl::unexpected;
#else
    using std::expected;
    using std::unexpected;
#endif
} // namespace melinda::mblcxx

#endif
