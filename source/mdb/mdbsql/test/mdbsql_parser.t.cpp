#include <catch2/catch_test_macros.hpp>

#include <mblcxx_always_false.h>

#include <mdbsql_ast.h>
#include <mdbsql_parser.h>

using namespace melinda;

TEST_CASE("<SQL executable statement>", "[sql-grammar]")
{
    using namespace std::string_literals;
    using namespace std::string_view_literals;

    SECTION("<schema definiton>")
    {
        auto as_schema_definition =
            [](mdbsql::ast::sql_executable_statement const& value)
        {
            return std::get<mdbsql::ast::schema_definition>(
                std::get<mdbsql::ast::sql_schema_statement>(value));
        };

        SECTION("Basic <schema definition>")
        {
            mdbsql::ast::identifier const unqualified_schema_name{{"schm"s}};
            mdbsql::ast::identifier const qualified_schema_name{
                {"ctlg"s, "schm"s}};

            auto queries_with_unqualified_name = {"CREATE SCHEMA schm"sv,
                "create schema schm"sv,
                "creAte ScheMa schm"sv};
            for (auto const& query : queries_with_unqualified_name)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& schema_definition =
                        as_schema_definition(*result);
                    REQUIRE(schema_definition.schema_name ==
                        unqualified_schema_name);
                    REQUIRE_FALSE(schema_definition.authorization);
                    REQUIRE_FALSE(schema_definition.character_set_or_path1);
                    REQUIRE_FALSE(schema_definition.character_set_or_path2);
                    REQUIRE_FALSE(schema_definition.elements);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }

            auto queries_with_qualified_name = {"CREATE SCHEMA ctlg.schm"sv,
                "create schema ctlg.schm"sv,
                "creAte ScheMa ctlg.schm"sv};
            for (auto const& query : queries_with_qualified_name)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& schema_definition =
                        as_schema_definition(*result);
                    REQUIRE(
                        schema_definition.schema_name == qualified_schema_name);
                    REQUIRE_FALSE(schema_definition.authorization);
                    REQUIRE_FALSE(schema_definition.character_set_or_path1);
                    REQUIRE_FALSE(schema_definition.character_set_or_path2);
                    REQUIRE_FALSE(schema_definition.elements);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }
        }

        SECTION("<schema definition> with AUTHORIZATION clause")
        {
            mdbsql::ast::identifier const schema_name{{"schm"s}};
            std::string const authorization{"auth"s};

            auto queries_with_schema_name = {
                "CREATE SCHEMA schm AUTHORIZATION auth"sv,
                "CREATE SCHEMA schm authorization auth"sv,
                "CREATE SCHEMA schm AutHorIzatIon auth"sv};
            for (auto const& query : queries_with_schema_name)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& schema_definition =
                        as_schema_definition(*result);
                    REQUIRE(schema_definition.schema_name == schema_name);
                    REQUIRE(schema_definition.authorization == authorization);
                    REQUIRE_FALSE(schema_definition.character_set_or_path1);
                    REQUIRE_FALSE(schema_definition.character_set_or_path2);
                    REQUIRE_FALSE(schema_definition.elements);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }

            auto queries_without_schema_name = {
                "CREATE SCHEMA AUTHORIZATION auth"sv,
                "CREATE SCHEMA authorization auth"sv,
                "CREATE SCHEMA AutHorIzatIon auth"sv};
            for (auto const& query : queries_without_schema_name)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& schema_definition =
                        as_schema_definition(*result);
                    REQUIRE_FALSE(schema_definition.schema_name);
                    REQUIRE(schema_definition.authorization);
                    REQUIRE(schema_definition.authorization == authorization);
                    REQUIRE_FALSE(schema_definition.character_set_or_path1);
                    REQUIRE_FALSE(schema_definition.character_set_or_path2);
                    REQUIRE_FALSE(schema_definition.elements);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }
        }

        SECTION("<schema definition> with character set specification")
        {
            mdbsql::ast::identifier const schema_name{{"schm"s}};
            mdbsql::ast::identifier const qualified_character_set{
                {"schm2"s, "chset"s}};
            mdbsql::ast::identifier const unqualified_character_set{{"chset"s}};

            auto queries_with_qualified_character_set = {
                "CREATE SCHEMA schm DEFAULT CHARACTER SET schm2.chset"sv,
                "CREATE SCHEMA schm default character set schm2.chset"sv,
                "CREATE SCHEMA schm DefAuLt CharActEr SeT schm2.chset"sv};
            for (auto const& query : queries_with_qualified_character_set)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& schema_definition =
                        as_schema_definition(*result);
                    REQUIRE(schema_definition.schema_name == schema_name);
                    REQUIRE_FALSE(schema_definition.authorization);
                    REQUIRE(schema_definition.character_set_or_path1);
                    auto const& chset = std::get<mdbsql::ast::identifier>(
                        schema_definition.character_set_or_path1.value());
                    REQUIRE(chset == qualified_character_set);
                    REQUIRE_FALSE(schema_definition.character_set_or_path2);
                    REQUIRE_FALSE(schema_definition.elements);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }

            auto queries_with_unqualified_character_set = {
                "CREATE SCHEMA schm DEFAULT CHARACTER SET chset"sv,
                "CREATE SCHEMA schm default character set chset"sv,
                "CREATE SCHEMA schm DefAuLt CharActEr SeT chset"sv};
            for (auto const& query : queries_with_unqualified_character_set)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& schema_definition =
                        as_schema_definition(*result);
                    REQUIRE(schema_definition.schema_name == schema_name);
                    REQUIRE_FALSE(schema_definition.authorization);
                    REQUIRE(schema_definition.character_set_or_path1);
                    auto const& chset = std::get<mdbsql::ast::identifier>(
                        schema_definition.character_set_or_path1.value());
                    REQUIRE(chset == unqualified_character_set);
                    REQUIRE_FALSE(schema_definition.character_set_or_path2);
                    REQUIRE_FALSE(schema_definition.elements);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }
        }

        SECTION("<schema definition> with path specification")
        {
            mdbsql::ast::identifier const schema_name{{"schm"s}};
            std::vector<mdbsql::ast::identifier> const path_specification{
                mdbsql::ast::identifier{{"schm1"s}},
                mdbsql::ast::identifier{{"ctlg"s, "schm2"s}}};

            auto queries_with_path_specification = {
                "CREATE SCHEMA schm PATH schm1, ctlg.schm2"sv,
                "CREATE SCHEMA schm path schm1, ctlg.schm2"sv,
                "CREATE SCHEMA schm pATh schm1, ctlg.schm2"sv};
            for (auto const& query : queries_with_path_specification)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& schema_definition =
                        as_schema_definition(*result);
                    REQUIRE(schema_definition.schema_name == schema_name);
                    REQUIRE_FALSE(schema_definition.authorization);
                    REQUIRE(schema_definition.character_set_or_path1);
                    auto const& path =
                        std::get<std::vector<mdbsql::ast::identifier>>(
                            schema_definition.character_set_or_path1.value());
                    REQUIRE(path.size() == 2);
                    REQUIRE(path[0] == path_specification[0]);
                    REQUIRE(path[1] == path_specification[1]);
                    REQUIRE_FALSE(schema_definition.character_set_or_path2);
                    REQUIRE_FALSE(schema_definition.elements);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }
        }

        SECTION("<schema definition> with character set and path specification")
        {
            mdbsql::ast::identifier const schema_name{{"schm"s}};
            mdbsql::ast::identifier const character_set{{"schm2"s, "chset"s}};
            std::vector<mdbsql::ast::identifier> const path_specification{
                mdbsql::ast::identifier{{"schm1"s}},
                mdbsql::ast::identifier{{"ctlg"s, "schm2"s}}};

            auto const visitor = [&character_set, &path_specification](auto&& v)
            {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, mdbsql::ast::identifier>)
                {
                    REQUIRE(v == character_set);
                }
                else if constexpr (std::is_same_v<T,
                                       std::vector<mdbsql::ast::identifier>>)
                {
                    REQUIRE(v.size() == 2);
                    REQUIRE(v[0] == path_specification[0]);
                    REQUIRE(v[1] == path_specification[1]);
                }
                else
                {
                    static_assert(mblcxx::always_false_v<T>,
                        "non-exhaustive visitor!");
                }
            };

            auto queries_with_character_set_and_path_specification = {
                "CREATE SCHEMA schm DEFAULT CHARACTER SET schm2.chset PATH schm1, ctlg.schm2"sv,
                "CREATE SCHEMA schm PATH schm1, ctlg.schm2 DEFAULT CHARACTER SET schm2.chset"sv};
            for (auto const& query :
                queries_with_character_set_and_path_specification)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& schema_definition =
                        as_schema_definition(*result);
                    REQUIRE(schema_definition.schema_name == schema_name);
                    REQUIRE_FALSE(schema_definition.authorization);
                    REQUIRE(schema_definition.character_set_or_path1);
                    std::visit(visitor,
                        *schema_definition.character_set_or_path1);
                    std::visit(visitor,
                        *schema_definition.character_set_or_path2);
                    REQUIRE_FALSE(schema_definition.elements);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }
        }

        SECTION("<schema definition> with <table definition>")
        {
            mdbsql::ast::identifier const schema_name{{"schm"s}};
            mdbsql::ast::identifier const qualified_table_name{
                {"ctlg"s, "schm"s, "tbl"s}};
            mdbsql::ast::identifier const unqualified_table_name{{"tbl"s}};

            auto queries_with_qualified_table_name = {
                "CREATE SCHEMA schm CREATE TABLE ctlg.schm.tbl(col1 integer, col2 integer)"sv,
                "CREATE SCHEMA schm create table ctlg.schm.tbl(col1 integer, col2 integer)"sv,
                "CREATE SCHEMA schm creAte TabLe ctlg.schm.tbl(col1 integer, col2 integer)"sv};
            for (auto const& query : queries_with_qualified_table_name)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& schema_definition =
                        as_schema_definition(*result);
                    REQUIRE(schema_definition.schema_name == schema_name);
                    REQUIRE_FALSE(schema_definition.authorization);
                    REQUIRE_FALSE(schema_definition.character_set_or_path1);
                    REQUIRE_FALSE(schema_definition.character_set_or_path2);
                    REQUIRE(schema_definition.elements);
                    auto const& table = std::get<mdbsql::ast::table_definition>(
                        schema_definition.elements.value());
                    REQUIRE(table.name == qualified_table_name);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }

            auto queries_with_unqualified_table_name = {
                "CREATE SCHEMA schm CREATE TABLE tbl(col1 integer, col2 integer)"sv,
                "CREATE SCHEMA schm create table tbl(col1 integer, col2 integer)"sv,
                "CREATE SCHEMA schm creAte TabLe tbl(col1 integer, col2 integer)"sv};
            for (auto const& query : queries_with_unqualified_table_name)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& schema_definition =
                        as_schema_definition(*result);
                    REQUIRE(schema_definition.schema_name == schema_name);
                    REQUIRE_FALSE(schema_definition.authorization);
                    REQUIRE_FALSE(schema_definition.character_set_or_path1);
                    REQUIRE_FALSE(schema_definition.character_set_or_path2);
                    REQUIRE(schema_definition.elements);
                    auto const& table = std::get<mdbsql::ast::table_definition>(
                        schema_definition.elements.value());
                    REQUIRE(table.name == unqualified_table_name);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }
        }
    }

    SECTION("<table definition>")
    {
        auto as_table_definition =
            [](mdbsql::ast::sql_executable_statement const& value)
        {
            return std::get<mdbsql::ast::table_definition>(
                std::get<mdbsql::ast::sql_schema_statement>(value));
        };

        SECTION("Basic <table definition>")
        {
            mdbsql::ast::identifier const qualified_table_name{
                {"ctlg"s, "schm"s, "tbl"s}};
            mdbsql::ast::identifier const unqualified_table_name{{"tbl"s}};

            auto queries_with_qualified_table_name = {
                "CREATE TABLE ctlg.schm.tbl(col1 integer, col2 integer)"sv,
                "create table ctlg.schm.tbl(col1 integer, col2 integer)"sv,
                "creAte TabLe ctlg.schm.tbl(col1 integer, col2 integer)"sv};
            for (auto const& query : queries_with_qualified_table_name)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& table_definition = as_table_definition(*result);
                    REQUIRE(table_definition.name == qualified_table_name);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }

            auto queries_with_unqualified_table_name = {
                "CREATE TABLE tbl(col1 integer, col2 integer)"sv,
                "create table tbl(col1 integer, col2 integer)"sv,
                "creAte TabLe tbl(col1 integer, col2 integer)"sv};
            for (auto const& query : queries_with_unqualified_table_name)
            {
                if (auto const result = mdbsql::parse(query); result)
                {
                    auto const& table_definition = as_table_definition(*result);
                    REQUIRE(table_definition.name == unqualified_table_name);
                }
                else
                {
                    FAIL("Parsing of query failed: " << query);
                }
            }
        }
    }
}
