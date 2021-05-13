#include <catch2/catch.hpp>

#include <mblcxx_scope_exit.h>

TEST_CASE("Exiting the scope calls the defined action")
{
    int value = 0;
    {
        MBLCXX_ON_SCOPE_EXIT(++value);
    }
    REQUIRE(value == 1);
}
