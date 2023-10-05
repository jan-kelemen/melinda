#ifndef MELINDA_MDBSQL_PARSER_COMMON_INCLUDED
#define MELINDA_MDBSQL_PARSER_COMMON_INCLUDED

#include <lexy/dsl/literal.hpp>
#include <lexy/dsl/newline.hpp>
#include <lexy/dsl/unicode.hpp>
#include <lexy/dsl/until.hpp>

namespace melinda::mdbsql::parser
{
    constexpr auto separator = lexy::dsl::lit<"/*"> >>
            lexy::dsl::until(lexy::dsl::lit<"*/">) |
        lexy::dsl::lit<"--"> >> lexy::dsl::until(lexy::dsl::newline) |
        lexy::dsl::unicode::space;
} // namespace melinda::mdbsql::parser

#endif
