#include <catch2/catch_test_macros.hpp>

#include <initializer_list>
#include <string>
#include <string_view>

#include <fmt/core.h>

#include <mblcxx_result.h>

#include <mqlprs_ast_delimited_identifier.h>
#include <mqlprs_delimited_identifier.h> // IWYU pragma: keep
#include <mqlprs_parser.h>

using namespace melinda;

namespace
{
    auto parse = [](std::string_view query)
    { return mqlprs::parse<mqlprs::ast::delimited_identifier>(query); };
} // namespace

TEST_CASE("<delimited identifer> escapes double quote symbol")
{
    using namespace std::string_view_literals;

    auto const result = parse(R"("before""after")"sv);
    REQUIRE(result);
    REQUIRE(result.ok().body == "before\"after");
}

TEST_CASE("<delimited identifier> allows usage of reserved word")
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
        auto const result{parse(fmt::format("\"{}\"", reserved_word))};

        if (!result)
        {
            FAIL("Parsing failed for reserved word: " << reserved_word);
        }

        REQUIRE(result);
        REQUIRE(result.ok().body == reserved_word);
    }
}
