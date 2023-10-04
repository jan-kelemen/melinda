#include <catch2/catch_test_macros.hpp>

#include <lexy/action/parse.hpp>
#include <lexy/action/trace.hpp>
#include <lexy/action/validate.hpp>
#include <lexy/encoding.hpp>
#include <lexy/input/string_input.hpp>

#include <mblcxx_result.h>

#include <mdbsql_ast_regular_identifier.h>
#include <mdbsql_parser_parse_error.h>
#include <mdbsql_parser_regular_identifier.h>

using namespace melinda;

mblcxx::result<mdbsql::ast::regular_identifier, mdbsql::parser::parse_error>
parse(std::string_view value)
{
    lexy::string_input<lexy::utf8_char_encoding> const range{value};

    if (auto result{lexy::parse<mdbsql::parser::regular_identifier>(range,
            lexy::collect<std::vector<mdbsql::parser::parse_error_detail>>(
                mdbsql::parser::error_callback{}))};
        result.has_value() && result.is_success())
    {
        return {std::in_place_index<0>, std::move(result).value()};
    }
    else
    {
        mdbsql::parser::parse_error error{std::move(result).errors()};
        return {std::in_place_index<1>, std::move(error)};
    }
}

TEST_CASE("<regular identifier> includes character classes", "[sql-grammar]")
{
    using namespace std::string_view_literals;

    SECTION("<identifier start> includes Uppercase Letter character class")
    {
        auto uppercase_identifiers = {"A"sv, "Σ"sv};
        for (auto&& id : uppercase_identifiers)
        {
            auto result = parse(id);

            REQUIRE(result);
            REQUIRE(result.ok().body == id);
        }
    }

    SECTION("<identifier start> includes Lowercase Letter character class")
    {
        auto lowercase_identifiers = {"a"sv, "ȹ"sv};
        for (auto&& id : lowercase_identifiers)
        {
            auto result = parse(id);

            REQUIRE(result);
            REQUIRE(result.ok().body == id);
        }
    }

    SECTION("<identifier start> includes Titlecase Letter character class")
    {
        auto titlecase_identifiers = {"ῼ"sv, "ǈ"sv};
        for (auto&& id : titlecase_identifiers)
        {
            auto result = parse(id);

            REQUIRE(result);
            REQUIRE(result.ok().body == id);
        }
    }

    SECTION("<identifier start> includes Modifier Letter character class")
    {
        auto titlecase_identifiers = {"ᵝ"sv, "ꟹ"sv};
        for (auto&& id : titlecase_identifiers)
        {
            auto result = parse(id);

            REQUIRE(result);
            REQUIRE(result.ok().body == id);
        }
    }

    SECTION("<identifier start> includes Other Letter character class")
    {
        auto titlecase_identifiers = {"ګ"sv, "ܤ"sv};
        for (auto&& id : titlecase_identifiers)
        {
            auto result = parse(id);

            REQUIRE(result);
            REQUIRE(result.ok().body == id);
        }
    }

    SECTION("<identifier start> includes Letter Number character class")
    {
        auto titlecase_identifiers = {"ⅻ"sv, "〥"sv};
        for (auto&& id : titlecase_identifiers)
        {
            auto result = parse(id);

            REQUIRE(result);
            REQUIRE(result.ok().body == id);
        }
    }

    SECTION("<identifier extend> includes Middle Dot character")
    {
        auto id = "a·"sv;

        auto result = parse(id);

        REQUIRE(result);
        REQUIRE(result.ok().body == id);
    }

    SECTION("<identifier extend> includes Letter Number character class")
    {
        auto id = "a\xCC\x83"sv;

        auto result = parse(id);

        REQUIRE(result);
        REQUIRE(result.ok().body == id);
    }

    SECTION("<identifier extend> includes Spacing Mark character class")
    {
        auto id = "a\xE0\xA7\x80"sv;

        auto result = parse(id);

        REQUIRE(result);
        REQUIRE(result.ok().body == id);
    }

    SECTION("<identifier extend> includes Decimal Number character class")
    {
        auto id = "a\xE0\xB9\x90"sv;

        auto result = parse(id);

        REQUIRE(result);
        REQUIRE(result.ok().body == id);
    }

    SECTION(
        "<identifier extend> includes Connector Punctuation character class")
    {
        auto id = "a\xEF\xB9\x8F"sv;

        auto result = parse(id);

        REQUIRE(result);
        REQUIRE(result.ok().body == id);
    }

    SECTION("<identifier extend> includes Format character class")
    {
        auto id = "a\xF3\xA0\x80\xB0"sv;

        auto result = parse(id);

        REQUIRE(result);
        REQUIRE(result.ok().body == id);
    }

    auto identifier_with_all_character_classes =
        "Aaǈꟹܤ〥·\xCC\x83\xE0\xA7\x80\xE0\xB9\x90\xEF\xB9\x8F\xF3\xA0\x80\xB0"sv;

    auto result = parse(identifier_with_all_character_classes);
    REQUIRE(result);
    REQUIRE(result.ok().body ==
        "Aaǈꟹܤ〥·\xCC\x83\xE0\xA7\x80\xE0\xB9\x90\xEF\xB9\x8F\xF3\xA0\x80\xB0");
}

TEST_CASE("<regular identifier> does not match a reserved word")
{
    auto reserved_words = {"ABS",
        "ALL",
        "ALLOCATE",
        "ALTER",
        "AND",
        "ANY",
        "ARE",
        "ARRAY",
        "ARRAY_AGG",
        "ARRAY_MAX_CARDINALITY",
        "AS",
        "ASENSITIVE",
        "ASYMMETRIC",
        "AT",
        "ATOMIC",
        "AUTHORIZATION",
        "AVG",
        "BEGIN",
        "BEGIN_FRAME",
        "BEGIN_PARTITION",
        "BETWEEN",
        "BIGINT",
        "BINARY",
        "BLOB",
        "BOOLEAN",
        "BOTH",
        "BY",
        "CALL",
        "CALLED",
        "CARDINALITY",
        "CASCADED",
        "CASE",
        "CAST",
        "CEIL",
        "CEILING",
        "CHAR",
        "CHARACTER",
        "CHARACTER_LENGTH",
        "CHAR_LENGTH",
        "CHECK",
        "CLOB",
        "CLOSE",
        "COALESCE",
        "COLLATE",
        "COLLECT",
        "COLUMN",
        "COMMIT",
        "CONDITION",
        "CONNECT",
        "CONSTRAINT",
        "CONTAINS",
        "CONVERT",
        "CORR",
        "CORRESPONDING",
        "COUNT",
        "COVAR_POP",
        "COVAR_SAMP",
        "CREATE",
        "CROSS",
        "CUBE",
        "CUME_DIST",
        "CURRENT",
        "CURRENT_CATALOG",
        "CURRENT_DATE",
        "CURRENT_DEFAULT_TRANSFORM_GROUP",
        "CURRENT_PATH",
        "CURRENT_ROLE",
        "CURRENT_ROW",
        "CURRENT_SCHEMA",
        "CURRENT_TIME",
        "CURRENT_TIMESTAMP",
        "CURRENT_TRANSFORM_GROUP_FOR_TYPE",
        "CURRENT_USER",
        "CURSOR",
        "CYCLE",
        "DATE",
        "DAY",
        "DEALLOCATE",
        "DEC",
        "DECIMAL",
        "DECLARE",
        "DEFAULT",
        "DELETE",
        "DENSE_RANK",
        "DEREF",
        "DESCRIBE",
        "DETERMINISTIC",
        "DISCONNECT",
        "DISTINCT",
        "DOUBLE",
        "DROP",
        "DYNAMIC",
        "EACH",
        "ELEMENT",
        "ELSE",
        "END",
        "END-EXEC",
        "END_FRAME",
        "END_PARTITION",
        "EQUALS",
        "ESCAPE",
        "EVERY",
        "EXCEPT",
        "EXEC",
        "EXECUTE",
        "EXISTS",
        "EXP",
        "EXTERNAL",
        "EXTRACT",
        "FALSE",
        "FETCH",
        "FILTER",
        "FIRST_VALUE",
        "FLOAT",
        "FLOOR",
        "FOR",
        "FOREIGN",
        "FRAME_ROW",
        "FREE",
        "FROM",
        "FULL",
        "FUNCTION",
        "FUSION",
        "GET",
        "GLOBAL",
        "GRANT",
        "GROUP",
        "GROUPING",
        "GROUPS",
        "HAVING",
        "HOLD",
        "HOUR",
        "IDENTITY",
        "IN",
        "INDICATOR",
        "INNER",
        "INOUT",
        "INSENSITIVE",
        "INSERT",
        "INT",
        "INTEGER",
        "INTERSECT",
        "INTERSECTION",
        "INTERVAL",
        "INTO",
        "IS",
        "JOIN",
        "LAG",
        "LANGUAGE",
        "LARGE",
        "LAST_VALUE",
        "LATERAL",
        "LEAD",
        "LEADING",
        "LEFT",
        "LIKE",
        "LIKE_REGEX",
        "LN",
        "LOCAL",
        "LOCALTIME",
        "LOCALTIMESTAMP",
        "LOWER",
        "MATCH",
        "MAX",
        "MEMBER",
        "MERGE",
        "METHOD",
        "MIN",
        "MINUTE",
        "MOD",
        "MODIFIES",
        "MODULE",
        "MONTH",
        "MULTISET",
        "NATIONAL",
        "NATURAL",
        "NCHAR",
        "NCLOB",
        "NEW",
        "NO",
        "NONE",
        "NORMALIZE",
        "NOT",
        "NTH_VALUE",
        "NTILE",
        "NULL",
        "NULLIF",
        "NUMERIC",
        "OCCURRENCES_REGEX",
        "OCTET_LENGTH",
        "OF",
        "OFFSET",
        "OLD",
        "ON",
        "ONLY",
        "OPEN",
        "OR",
        "ORDER",
        "OUT",
        "OUTER",
        "OVER",
        "OVERLAPS",
        "OVERLAY",
        "PARAMETER",
        "PARTITION",
        "PERCENT",
        "PERCENTILE_CONT",
        "PERCENTILE_DISC",
        "PERCENT_RANK",
        "PERIOD",
        "PORTION",
        "POSITION",
        "POSITION_REGEX",
        "POWER",
        "PRECEDES",
        "PRECISION",
        "PREPARE",
        "PRIMARY",
        "PROCEDURE",
        "RANGE",
        "RANK",
        "READS",
        "REAL",
        "RECURSIVE",
        "REF",
        "REFERENCES",
        "REFERENCING",
        "REGR_AVGX",
        "REGR_AVGY",
        "REGR_COUNT",
        "REGR_INTERCEPT",
        "REGR_R2",
        "REGR_SLOPE",
        "REGR_SXX",
        "REGR_SXY",
        "REGR_SYY",
        "RELEASE",
        "RESULT",
        "RETURN",
        "RETURNS",
        "REVOKE",
        "RIGHT",
        "ROLLBACK",
        "ROLLUP",
        "ROW",
        "ROWS",
        "ROW_NUMBER",
        "SAVEPOINT",
        "SCOPE",
        "SCROLL",
        "SEARCH",
        "SECOND",
        "SELECT",
        "SENSITIVE",
        "SESSION_USER",
        "SET",
        "SIMILAR",
        "SMALLINT",
        "SOME",
        "SPECIFIC",
        "SPECIFICTYPE",
        "SQL",
        "SQLEXCEPTION",
        "SQLSTATE",
        "SQLWARNING",
        "SQRT",
        "START",
        "STATIC",
        "STDDEV_POP",
        "STDDEV_SAMP",
        "SUBMULTISET",
        "SUBSTRING",
        "SUBSTRING_REGEX",
        "SUCCEEDS",
        "SUM",
        "SYMMETRIC",
        "SYSTEM",
        "SYSTEM_TIME",
        "SYSTEM_USER",
        "TABLE",
        "TABLESAMPLE",
        "THEN",
        "TIME",
        "TIMESTAMP",
        "TIMEZONE_HOUR",
        "TIMEZONE_MINUTE",
        "TO",
        "TRAILING",
        "TRANSLATE",
        "TRANSLATE_REGEX",
        "TRANSLATION",
        "TREAT",
        "TRIGGER",
        "TRIM",
        "TRIM_ARRAY",
        "TRUE",
        "TRUNCATE",
        "UESCAPE",
        "UNION",
        "UNIQUE",
        "UNKNOWN",
        "UNNEST",
        "UPDATE",
        "UPPER",
        "USER",
        "USING",
        "VALUE",
        "VALUES",
        "VALUE_OF",
        "VARBINARY",
        "VARCHAR",
        "VARYING",
        "VAR_POP",
        "VAR_SAMP",
        "VERSIONING",
        "WHEN",
        "WHENEVER",
        "WHERE",
        "WIDTH_BUCKET",
        "WINDOW",
        "WITH",
        "WITHIN",
        "WITHOUT",
        "YEAR"};

    for (auto&& reserved_word : reserved_words)
    {
        auto result{parse(reserved_word)};

        if (result)
        {
            FAIL("Parsing succeed for reserved word: " << reserved_word);
        }

        REQUIRE_FALSE(result);
    }
}
