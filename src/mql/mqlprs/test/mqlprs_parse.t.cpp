#include <mqlprs_parse.hpp>

#include <mblcxx_result.hpp>
#include <mqlprs_tree_util.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <lexy/parse_tree.hpp>

#include <iterator>

// IWYU pragma: no_include <lexy/_detail/iterator.hpp>
// IWYU pragma: no_include <lexy/input/base.hpp>
// IWYU pragma: no_include <ranges>

using namespace melinda;

namespace
{
    [[nodiscard]] constexpr auto count(auto&& range)
    {
        return std::distance(range.cbegin(), range.cend());
    }
} // namespace

TEST_CASE("mqlprs::parse statement parsing")
{
    SECTION("Empty statements can be parsed successfully")
    {
        auto const* const empty_statement{GENERATE("",
            ";",
            " ;",
            "; ",
            "/* */;",
            ";/* */",
            "-- abc",
            "-- a\n;",
            ";;")};
        auto const result{mqlprs::parse(empty_statement)};
        REQUIRE(result);
    }

    SECTION("Statements are separated by semicolons")
    {
        auto const result{mqlprs::parse("create schema a; create schema b")};
        REQUIRE(result);
        REQUIRE(
            count(mqlprs::tree_util::children_of_type<mqlprs::create_statement>(
                result->root())) == 2);
    }

    SECTION("Semicolon is optional for lprs statement")
    {
        auto const* const single_statement{GENERATE("create schema a",
            "create schema a;",
            "create schema a\n",
            "create schema a\n;",
            "create schema a--meow")};
        auto const result{mqlprs::parse(single_statement)};
        REQUIRE(result);
    }
}

TEST_CASE("mqlprs::parse case sensitivity")
{
    SECTION("mqlprs::create_statement is case insensitive")
    {
        auto const* const statement{GENERATE("create schema a",
            "create SCHEMA a",
            "CREATE schema a",
            "crAtE SchEma a")};
        auto const result{mqlprs::parse(statement)};
        REQUIRE(result);
    }
}
