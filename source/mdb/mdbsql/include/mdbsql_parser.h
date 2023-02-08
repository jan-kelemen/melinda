#ifndef MELINDA_MDBSQL_PARSER_INCLUDED
#define MELINDA_MDBSQL_PARSER_INCLUDED

#include <algorithm>
#include <lexy/dsl/brackets.hpp>
#include <lexy/dsl/separator.hpp>
#include <vector>

#include <lexy/callback/container.hpp>
#include <lexy/callback/fold.hpp>
#include <lexy/callback/string.hpp>
#include <lexy/dsl.hpp>
#include <lexy/dsl/ascii.hpp>
#include <lexy/dsl/case_folding.hpp>
#include <lexy/dsl/follow.hpp>
#include <lexy/dsl/identifier.hpp>
#include <lexy/dsl/newline.hpp>
#include <lexy/dsl/option.hpp>
#include <lexy/dsl/punctuator.hpp>
#include <lexy/dsl/times.hpp>
#include <lexy/dsl/unicode.hpp>
#include <lexy/encoding.hpp>
#include <lexy/input/buffer.hpp>

#include <mdbsql_ast.h>

namespace melinda::mdbsql::grammar
{
    namespace dsl = lexy::dsl;

    constexpr auto separator = LEXY_LIT("/*") >> dsl::until(LEXY_LIT("*/")) |
        LEXY_LIT("--") >> dsl::until(dsl::newline) | dsl::unicode::space;

    constexpr auto kw =
        dsl::identifier(dsl::ascii::alpha, dsl::ascii::alpha_underscore);

    struct identifier_part
    {
        static constexpr auto rule = []
        {
            auto lead_char = dsl::ascii::alpha;
            auto trailing_char = dsl::ascii::alpha_underscore;
            auto id = dsl::identifier(lead_char, trailing_char);

            return id.reserve(
                dsl::ascii::case_folding(LEXY_KEYWORD(u8"authorization", kw)));
        }();

        static constexpr auto value =
            lexy::as_string<std::string, lexy::utf8_encoding>;
    };

    struct identifier
    {
        static constexpr auto rule = []
        { return dsl::list(dsl::p<identifier_part>, dsl::sep(dsl::period)); }();

        static constexpr auto value = lexy::fold_inplace<ast::identifier>(
            ast::identifier {},
            [](auto& rv, std::string i) { rv.parts.push_back(std::move(i)); });
    };

    struct column_definition
    {
        static constexpr auto rule = []
        { return dsl::p<identifier> + dsl::p<identifier_part>; }();
        static constexpr auto value = lexy::construct<ast::column_definition>;
    };

    struct table_elements
    {
        static constexpr auto rule = []
        {
            return dsl::parenthesized.list(dsl::p<column_definition>,
                dsl::sep(dsl::comma));
        }();

        static constexpr auto value =
            lexy::fold_inplace<ast::table_elements>({},
                [](auto& rv, ast::column_definition v)
                { rv.elements.push_back(std::move(v)); });
    };

    struct table_definition
    {
        static constexpr auto rule = []
        {
            return dsl::ascii::case_folding(LEXY_KEYWORD(u8"create", kw)) >>
                dsl::opt(dsl::p<identifier_part> >>
                    dsl::ascii::case_folding(LEXY_KEYWORD(u8"temporary", kw))) +
                dsl::ascii::case_folding(LEXY_KEYWORD(u8"table", kw)) +
                dsl::p<identifier> + dsl::p<table_elements>;
        }();
        static constexpr auto value = lexy::construct<ast::table_definition>;
    };

    struct path_specification
    {
        static constexpr auto rule = []
        { return dsl::list(dsl::p<identifier>, dsl::sep(dsl::comma)); }();

        static constexpr auto value =
            lexy::fold_inplace<std::vector<ast::identifier>>({},
                [](auto& rv, ast::identifier v)
                { rv.push_back(std::move(v)); });
    };

    struct schema_definition
    {
        static constexpr auto rule = []
        {
            auto authorization =
                dsl::ascii::case_folding(LEXY_KEYWORD(u8"authorization", kw)) >>
                dsl::p<identifier_part>;

            auto character_set =
                dsl::ascii::case_folding(LEXY_KEYWORD(u8"default", kw)) >>
                dsl::ascii::case_folding(LEXY_KEYWORD(u8"character", kw)) >>
                dsl::ascii::case_folding(LEXY_KEYWORD(u8"set", kw)) >>
                dsl::p<identifier>;
            auto path_spec =
                dsl::ascii::case_folding(LEXY_KEYWORD(u8"paths", kw)) >>
                dsl::p<path_specification>;

            return dsl::ascii::case_folding(LEXY_KEYWORD(u8"create", kw)) >>
                dsl::ascii::case_folding(LEXY_KEYWORD(u8"schema", kw)) >>
                (dsl::ascii::case_folding(
                     LEXY_KEYWORD(u8"authorization", kw)) >>
                        dsl::nullopt + dsl::p<identifier_part> |
                    dsl::else_ >>
                        dsl::p<identifier> + dsl::opt(authorization)) +
                dsl::opt(character_set | path_spec) +
                dsl::opt(character_set | path_spec) +
                dsl::opt(dsl::p<table_definition>);
        }();

        static constexpr auto value = lexy::construct<ast::schema_definition>;
    };

    struct sql_schema_statement
    {
        static constexpr auto rule = dsl::p<schema_definition>;
        static constexpr auto value =
            lexy::construct<ast::sql_schema_statement>;
    };

    struct sql_executable_statement
    {
        static constexpr auto whitespace = separator;
        static constexpr auto rule = dsl::p<sql_schema_statement>;
        static constexpr auto value =
            lexy::construct<ast::sql_executable_statement>;
    };
} // namespace melinda::mdbsql::grammar

#endif
