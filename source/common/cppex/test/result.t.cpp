#include "../include/result.h"
#include "catch2/catch.hpp"

TEST_CASE("Result constructed from a value is considered not to be an error")
{
    mel::cppex::result<int> r = mel::cppex::result<int>(1);
    REQUIRE(static_cast<bool>(r));
}

TEST_CASE(
    "Result constructed from a std::error_code is considered to be an error")
{
    mel::cppex::result<int> r =
        mel::cppex::result<int>(std::make_error_code(std::errc::bad_address));
    REQUIRE(static_cast<bool>(r));
}

TEST_CASE(
    "std::error_condition and std::error_code are not allowed as values of result")
{
    static_assert(!mel::cppex::detail::is_result_value<std::error_code>);
    static_assert(!mel::cppex::detail::is_result_value<std::error_condition>);
}
