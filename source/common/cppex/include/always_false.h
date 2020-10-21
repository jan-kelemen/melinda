#ifndef MELINDA_COMMON_CPPEX_ALWAYS_FALSE_INCLUDED
#define MELINDA_COMMON_CPPEX_ALWAYS_FALSE_INCLUDED

namespace mel::cppex
{
    template<class>
    inline constexpr bool always_false_v = false;
}

#endif
