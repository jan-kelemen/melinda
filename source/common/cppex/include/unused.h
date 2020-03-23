#ifndef MELINDA_COMMON_CPPEX_UNUSED_INCLUDED
#define MELINDA_COMMON_CPPEX_UNUSED_INCLUDED

// Provides a utility for explicitly agreeing that a value is unused. A
//
// Although the C++17 attribute [[maybe_unused]] is nice, it can't be used
// simmetrically for function arguments and return values. Return values still
// have to be assigned and pollute the scope.

#define MELINDA_UNUSED(x) static_cast<void>(x)

#endif
