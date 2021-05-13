#include <catch2/catch.hpp>

#include <mbltrc_trace.h>

using namespace melinda;

TEST_CASE("Trace level always is larger than any other level")
{
    static_assert(mbltrc::trace_level::debug < mbltrc::trace_level::always);
    static_assert(mbltrc::trace_level::info < mbltrc::trace_level::always);
    static_assert(mbltrc::trace_level::warn < mbltrc::trace_level::always);
    static_assert(mbltrc::trace_level::error < mbltrc::trace_level::always);
    static_assert(mbltrc::trace_level::fatal < mbltrc::trace_level::always);
}
