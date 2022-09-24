#include <catch2/catch_test_macros.hpp>

#include <string_view>
#include <vector>

#include <mdbsql_grammar.h>

using namespace melinda;

TEST_CASE("CREATE statement parsing", "[sql]")
{
    std::vector<std::string_view> statements {
        "CREATE DATABASE books;",
        "CREATE DATABASE books",
    };

    for (auto const& statement : statements)
    {
        std::optional<mdbsql::statement> parsed;
        REQUIRE(mdbsql::parse(std::cbegin(statement),
            std::cend(statement),
            parsed));

        REQUIRE(parsed);
        REQUIRE(get<mdbsql::statement::create>(parsed->value).database_name ==
            "books");
    }
}
