#ifndef MELINDA_MBLCXX_ALWAYS_FALSE_INCLUDED
#define MELINDA_MBLCXX_ALWAYS_FALSE_INCLUDED

namespace melinda::mblcxx
{
    template<class>
    inline constexpr bool always_false_v = false;

}

#endif
