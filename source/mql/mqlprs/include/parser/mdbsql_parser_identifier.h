#ifndef MELINDA_MDBSQL_PARSER_IDENTIFIER_INCLUDED
#define MELINDA_MDBSQL_PARSER_IDENTIFIER_INCLUDED

#include <lexy/callback/container.hpp>
#include <lexy/callback/object.hpp>
#include <lexy/dsl/list.hpp>
#include <lexy/dsl/production.hpp>

#include <mdbsql_ast_identifier.h> // IWYU pragma: export
#include <mdbsql_parser_delimited_identifier.h>
#include <mdbsql_parser_regular_identifier.h>
#include <mdbsql_parser_unicode_delimited_identifier.h>

namespace melinda::mdbsql::parser
{
    struct [[nodiscard]] identifier final
    {
        static constexpr auto rule = lexy::dsl::p<regular_identifier> |
            lexy::dsl::p<delimited_identifier> |
            lexy::dsl::p<unicode_delimited_identifier>;

        static constexpr auto value = lexy::construct<ast::identifier>;
    };

    struct [[nodiscard]] multipart_identifier final
    {
        static constexpr auto rule = lexy::dsl::list(lexy::dsl::p<identifier>,
            lexy::dsl::sep(lexy::dsl::period));

        static constexpr auto value = lexy::as_list<ast::multipart_identifier>;
    };
} // namespace melinda::mdbsql::parser

#endif
