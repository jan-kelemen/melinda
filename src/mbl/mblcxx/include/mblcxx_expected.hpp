#ifndef MELINDA_MBLCXX_EXPECTED_INCLUDED
#define MELINDA_MBLCXX_EXPECTED_INCLUDED

#ifdef __has_include
#if __has_include(<expected>)
#ifdef __clang__
#include <tl/expected.hpp> // IWYU pragma: export
#else
#include <expected> // IWYU pragma: export
#endif
#endif
#endif

#include <system_error> // IWYU pragma: export

namespace melinda::mblcxx
{
#ifdef __clang__
    using tl::expected;
    using tl::unexpected;
#else
    using std::expected;
    using std::unexpected;
#endif
} // namespace melinda::mblcxx

#endif
