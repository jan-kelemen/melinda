#include <catch2/catch_test_macros.hpp>

#include <initializer_list>
#include <string>
#include <string_view>

#include <fmt/core.h>

#include <mqlprs_ast_unicode_delimited_identifier.h>
#include <mqlprs_parser.h>
#include <mqlprs_unicode_delimited_identifier.h> // IWYU pragma: keep

using namespace melinda;

namespace
{
    auto parse = [](std::string_view query)
    { return mqlprs::parse<mqlprs::ast::unicode_delimited_identifier>(query); };
} // namespace

TEST_CASE("<Unicode delimited identifer> escapes double quote symbol")
{
    using namespace std::string_view_literals;

    auto const result = parse(R"(U&"before""after")"sv);
    REQUIRE(result);
    REQUIRE(result->body == "before\"after");
}

TEST_CASE("<Unicode delimited identifier> introducer")
{
    using namespace std::string_view_literals;

    SECTION("Introducer is case insensitive")
    {
        auto const uppercase_introducer = R"(U&"before")"sv;
        auto const lowercase_introducer = R"(u&"before")"sv;

        auto const expected_parsed_value = "before";

        auto const uppercase_result = parse(uppercase_introducer);
        REQUIRE(uppercase_result);
        REQUIRE(uppercase_result->body == expected_parsed_value);

        auto const lowercase_result = parse(lowercase_introducer);
        REQUIRE(lowercase_result);
        REQUIRE(lowercase_result->body == expected_parsed_value);
    }

    SECTION("Introducer does not allow for spaces")
    {
        auto const introducers_with_spaces = {R"(U &"space")"sv,
            R"(U& "space")"sv};
        for (auto&& introducer : introducers_with_spaces)
        {
            auto const result = parse(introducer);
            REQUIRE_FALSE(result);
        }
    }
}

TEST_CASE("<Unicode delimited indentifier> escape character")
{
    using namespace std::string_view_literals;

    SECTION("Backslash is default escape character")
    {
        auto const result = parse(R"(U&"str\\")");
        REQUIRE(result);
        REQUIRE(result->body == "str\\");
        REQUIRE(result->escape_character == '\\');
    }

    SECTION("Can be specified with <Unicode escape specifier>")
    {
        auto const escape_specifiers = {R"(U&"str" UESCAPE 'y')"sv,
            R"(U&"str" uescape 'y')"sv};
        for (auto&& escape_specifier : escape_specifiers)
        {
            auto const result = parse(escape_specifier);
            REQUIRE(result);
            REQUIRE(result->escape_character == 'y');
        }
    }

    SECTION("<Unicode escape specifier> allows for optional <separator>")
    {
        auto const strings_with_separators = {R"(U&"str"UESCAPE'y'")"sv,
            R"(U&"str" UESCAPE'y'")"sv,
            R"(U&"str" UESCAPE 'y'")"sv,
            R"(U&"str"/**/UESCAPE/**/'y'")"sv,
            "U&\"str\"--\nUESCAPE'y'\""sv,
            "U&\"str\"--\nUESCAPE--\n'y'\""sv};

        for (auto&& str : strings_with_separators)
        {
            auto const result = parse(str);
            if (!result)
            {
                FAIL("Parsing failed for: " << str);
            }
            REQUIRE(result);
            REQUIRE(result->escape_character == 'y');
        }
    }

    SECTION("<Unicode escape specifier> fails with multiple characters")
    {
        auto const result = parse(R"(U&"str" UESCAPE 'yy')");
        REQUIRE_FALSE(result);
    }

    SECTION("<Unicode escape specifier> is respected in string literal")
    {
        auto const result = parse(R"(U&"d!0061t!+000061" UESCAPE '!')");
        REQUIRE(result);
        REQUIRE(result->body == "data");
    }

    SECTION(
        "Doubling <Unicode escape character> in string literal escapes escape character")
    {
        auto const result = parse(R"(U&"d!!" UESCAPE '!')");
        REQUIRE(result);
        REQUIRE(result->body == "d!");
    }

    SECTION("Parsing fails for invalid escape sequences")
    {
        auto const escape_character_followed_by_quote = R"(U&"\"")";
        auto const escape_character_followed_by_printable = R"(U&"\a")";
        auto const three_digit_unicode_escape = R"(U&"\000")";
        auto const five_digit_unicode_escape = R"(U&"\+00000")";

        auto const invalid_escape_sequences = {
            escape_character_followed_by_quote,
            escape_character_followed_by_printable,
            three_digit_unicode_escape,
            five_digit_unicode_escape};
        for (auto&& sequence : invalid_escape_sequences)
        {
            auto const result = parse(sequence);
            if (result)
            {
                FAIL("Parsing succeeded for: " << sequence);
            }
            REQUIRE_FALSE(result);
        }
    }
}

TEST_CASE(
    "Extra numbers after <Unicode escaped value> are parsed as normal text")
{
    auto const a0_strings = {R"(U&"\00610")", R"(U&"\+0000610")"};
    for (auto&& str : a0_strings)
    {
        auto const result = parse(str);
        REQUIRE(result);
        REQUIRE(result->body == "a0");
    }
}

TEST_CASE("<Unicode delimited identifier> allows usage of reserved word")
{
    auto const reserved_words = {"ABS",
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
        auto const result{parse(fmt::format("U&\"{}\"", reserved_word))};

        if (!result)
        {
            FAIL("Parsing failed for reserved word: " << reserved_word);
        }

        REQUIRE(result);
        REQUIRE(result->body == reserved_word);
    }
}
