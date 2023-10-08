#ifndef MELINDA_MDBSQL_PARSER_COMMON_INCLUDED
#define MELINDA_MDBSQL_PARSER_COMMON_INCLUDED

#include <lexy/dsl/branch.hpp>
#include <lexy/dsl/choice.hpp>
#include <lexy/dsl/identifier.hpp>
#include <lexy/dsl/literal.hpp>
#include <lexy/dsl/newline.hpp>
#include <lexy/dsl/unicode.hpp>
#include <lexy/dsl/until.hpp>

namespace melinda::mdbsql::parser
{
    constexpr auto separator = LEXY_LIT("/*") >>
            lexy::dsl::until(LEXY_LIT("*/")) |
        LEXY_LIT("--") >> lexy::dsl::until(lexy::dsl::newline) |
        lexy::dsl::unicode::space;

    constexpr auto kw = lexy::dsl::identifier(lexy::dsl::ascii::alpha,
        lexy::dsl::ascii::alpha_underscore / LEXY_LIT("-"));
} // namespace melinda::mdbsql::parser

#endif
