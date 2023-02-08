#include <catch2/catch_test_macros.hpp>

#include <lexy/action/parse.hpp>
#include <lexy/action/trace.hpp>
#include <lexy/encoding.hpp>
#include <lexy/input/range_input.hpp>
#include <lexy_ext/report_error.hpp>

#include <mdbsql_ast.h>
#include <mdbsql_parser.h>

using namespace melinda;

TEST_CASE("<SQL executable statement>", "[sql-grammar]")
{
    std::u8string const input {
        u8"CREATE/* fdsa*/SCHEMA melinda.mirko.kirko AUTHORIZATion mirko PATHS melinda.mirko, melinda.kirko default character set melinda.en_US CREATE global temporary table krokodil.mirko (foobar int, truebar varchar, kooobar boolean, kib.fda real)"};
    lexy::range_input<lexy::utf8_encoding, std::u8string::const_iterator> const
        range {std::cbegin(input), std::cend(input)};

    lexy::trace<mdbsql::grammar::sql_executable_statement>(stdout, range);
    auto const result =
        lexy::parse<mdbsql::grammar::sql_executable_statement>(range,
            lexy_ext::report_error);
    REQUIRE(result.is_success());
    REQUIRE(result.has_value());

    auto const& schema_definition = std::get<mdbsql::ast::schema_definition>(
        std::get<mdbsql::ast::sql_schema_statement>(result.value()));

    INFO(schema_definition.schema_name.value().parts[0]);
    REQUIRE(schema_definition.schema_name.value().parts[0] == "melinda");
    REQUIRE(schema_definition.authorization == "mirko");
}
