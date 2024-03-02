#include <mqlast_from_parse_tree.hpp>

#include <mqlast_syntax_tree.hpp>

#include <mblcxx_expected.hpp>
#include <mqlprs_parse.hpp>

#include <catch2/catch_test_macros.hpp>

#include <string_view>
#include <system_error>

// IWYU pragma: no_include <lexy/_detail/iterator.hpp>
// IWYU pragma: no_include <lexy/input/base.hpp>
// IWYU pragma: no_include <lexy/parse_tree.hpp>
// IWYU pragma: no_include <ranges>
// IWYU pragma: no_include <string>
// IWYU pragma: no_include <variant>
// IWYU pragma: no_include <vector>

using namespace melinda;

namespace
{
    mblcxx::expected<mqlast::syntax_tree, std::error_code> from_query(
        std::string_view query)
    {
        auto parse_tree{mqlprs::parse(query)};
        REQUIRE(parse_tree);
        return mqlast::from_parse_tree(*parse_tree);
    }
} // namespace

TEST_CASE("mqlast::from_parse_tree empty_tree")
{
    auto const ast{from_query("")};
    REQUIRE(ast);
    REQUIRE(ast->commands.empty());
}

TEST_CASE("mqlast::from_parse_tree create schema")
{
    SECTION("Single statement")
    {
        auto const ast{from_query("create schema a")};
        REQUIRE(ast);

        REQUIRE(ast->commands.size() == 1);

        auto const& command{
            std::get<mqlast::create_schema_command>(ast->commands.front())};
        REQUIRE(command.schema_name == "a");
    }
}
