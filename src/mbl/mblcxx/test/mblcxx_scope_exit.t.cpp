#include <mblcxx_scope_exit.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("mblcxx::on_scope_exit calls the defined action")
{
    int value{};
    {
        MBLCXX_ON_SCOPE_EXIT(++value);
    }
    REQUIRE(value == 1);
}
