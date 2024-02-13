#include <mblcxx_result.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace melinda;

TEST_CASE("mblcxx::result operator== supports conversions")
{
    constexpr int contained{2};

    mblcxx::result<int> result{contained};

    REQUIRE(result == contained);
    REQUIRE(contained == result);
}
