#include "../include/scope_exit.h"
#include "catch2/catch.hpp"

TEST_CASE("Exiting the scope calls the defined action")
{
    int value = 0;
    {
        ON_SCOPE_EXIT(++value);
    }
    REQUIRE(value == 1);
}
