#ifndef MELINDA_MDBSQL_PARSER_REGULAR_IDENTIFIER_INCLUDED
#define MELINDA_MDBSQL_PARSER_REGULAR_IDENTIFIER_INCLUDED

#include <lexy/callback/adapter.hpp>
#include <lexy/callback/object.hpp>
#include <lexy/code_point.hpp>
#include <lexy/dsl/code_point.hpp>
#include <lexy/dsl/identifier.hpp>
#include <lexy/encoding.hpp>

#include <mdbsql_ast_regular_identifier.h>
#include <mdbsql_parser_reserved_word.h>

namespace melinda::mdbsql::parser
{
    struct [[nodiscard]] regular_identifier final
    {
        static constexpr auto rule = []
        {
            auto identifier_start =
                lexy::dsl::code_point.general_category<lexy::code_point::Lu>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Ll>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Lt>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Lm>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Lo>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Nl>();

            auto identifier_extend = lexy::dsl::lit_cp<0x00B7> /
                lexy::dsl::code_point.general_category<lexy::code_point::Mn>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Mc>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Nd>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Pc>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Cf>();

            auto identifier_part = identifier_start / identifier_extend;

            // TODO-JK: Limit the number of <identifier parts> to < 128
            return lexy::dsl::identifier(identifier_start, identifier_part)
                .reserve(reserved_set1)
                .reserve(reserved_set2);
        }();

        static constexpr auto value = lexy::callback<ast::regular_identifier>(
            [](auto lexeme) {
                return ast::regular_identifier{{lexeme.begin(), lexeme.end()}};
            });
    };
} // namespace melinda::mdbsql::parser

#endif
