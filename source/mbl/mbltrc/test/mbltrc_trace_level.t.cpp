#include <catch2/catch_test_macros.hpp>

#include <mbltrc_trace_level.h>

TEST_CASE("Trace level always is larger than other trace levels")
{
    using melinda::mbltrc::trace_level;

    STATIC_REQUIRE(trace_level::debug < trace_level::always);
    STATIC_REQUIRE(trace_level::info < trace_level::always);
    STATIC_REQUIRE(trace_level::warn < trace_level::always);
    STATIC_REQUIRE(trace_level::error < trace_level::always);
    STATIC_REQUIRE(trace_level::fatal < trace_level::always);
}
