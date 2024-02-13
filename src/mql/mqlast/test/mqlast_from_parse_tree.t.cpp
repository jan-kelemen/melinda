#include <mqlast_from_parse_tree.hpp>

#include <mqlast_syntax_tree.hpp>

#include <mblcxx_result.hpp>
#include <mqlprs_parse.hpp>

#include <catch2/catch_test_macros.hpp>

#include <string_view>

// IWYU pragma: no_include <lexy/_detail/iterator.hpp>
// IWYU pragma: no_include <lexy/input/base.hpp>
// IWYU pragma: no_include <lexy/parse_tree.hpp>
// IWYU pragma: no_include <ranges>
// IWYU pragma: no_include <vector>

using namespace melinda;

namespace
{
    mblcxx::result<mqlast::syntax_tree> from_query(std::string_view query)
    {
        auto parse_tree{mqlprs::parse(query)};
        REQUIRE(parse_tree);
        return mqlast::from_parse_tree(*parse_tree);
    }
} // namespace

TEST_CASE("mqlast::from_parse_tree create_schema_command")
{
    SECTION("Single statement")
    {
        auto const ast{from_query("create schema a")};
        REQUIRE(ast);

        REQUIRE(ast->commands.size() == 1);
    }
}
