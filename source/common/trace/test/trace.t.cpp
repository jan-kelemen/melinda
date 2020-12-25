#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "trace.h"

TEST_CASE("Trace level always is larger than any other level")
{
    using mel::trace::trace_level;
    static_assert(trace_level::debug < trace_level::always);
    static_assert(trace_level::info < trace_level::always);
    static_assert(trace_level::warn < trace_level::always);
    static_assert(trace_level::error < trace_level::always);
    static_assert(trace_level::fatal < trace_level::always);
}
