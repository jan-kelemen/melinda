#ifndef MELINDA_MDBSQL_PARSER_UNICODE_DELIMITED_IDENTIFIER_INCLUDED
#define MELINDA_MDBSQL_PARSER_UNICODE_DELIMITED_IDENTIFIER_INCLUDED

#include <lexy/callback/adapter.hpp>
#include <lexy/code_point.hpp>
#include <lexy/dsl/ascii.hpp>
#include <lexy/dsl/capture.hpp>
#include <lexy/dsl/case_folding.hpp>
#include <lexy/dsl/code_point.hpp>
#include <lexy/dsl/digit.hpp>
#include <lexy/dsl/literal.hpp>
#include <lexy/dsl/punctuator.hpp>
#include <lexy/dsl/scan.hpp>
#include <lexy/dsl/sign.hpp>
#include <lexy/dsl/token.hpp>
#include <lexy/dsl/unicode.hpp>
#include <lexy/grammar.hpp>
#include <lexy/lexeme.hpp>

#include <mdbsql_ast_regular_identifier.h>
#include <mdbsql_parser_common.h>
#include <mdbsql_parser_reserved_word.h>

namespace melinda::mdbsql::parser
{
    struct [[nodiscard]] unicode_delimited_identifier final
        : lexy::scan_production<ast::unicode_delimited_identifier>
        , lexy::token_production
    {
        template<typename Context, typename Reader>
        static constexpr lexy::scan_result<ast::unicode_delimited_identifier>
        scan(lexy::rule_scanner<Context, Reader>& scanner)
        {
            auto everything_except_doublequote =
                lexy::dsl::capture(-lexy::dsl::lit_c<'"'>);

            auto unicode_escape_character =
                lexy::dsl::capture(-(lexy::dsl::digit<lexy::dsl::hex> /
                    lexy::dsl::hyphen / lexy::dsl::apostrophe /
                    lexy::dsl::lit_c<'"'> / lexy::dsl::unicode::space));

            auto unicode_4_digit_escape_value = unicode_escape_character +
                lexy::dsl::n_digits<4, lexy::dsl::hex>;

            auto unicode_6_digit_escape_value = unicode_escape_character +
                lexy::dsl::lit_c<'+'> + lexy::dsl::n_digits<6, lexy::dsl::hex>;

            scanner.parse(lexy::dsl::ascii::case_folding(lexy::dsl::lit<"u&">));
            if (!scanner)
            {
                return lexy::scan_failed;
            }

            scanner.parse(lexy::dsl::lit_c<'"'>);
            if (!scanner)
            {
                return lexy::scan_failed;
            }

            std::string result;

            auto found_closing_quote{false};
            while (!found_closing_quote)
            {
                if (scanner.branch(lexy::dsl::lit<"\"\"">))
                {
                    result += '"';
                }
                if (scanner.branch(lexy::dsl::lit_c<'"'>))
                {
                    found_closing_quote = true;
                }
                else if (scanner.is_at_eof())
                {
                    scanner.fatal_error(lexy::expected_char_class{},
                        scanner.begin(),
                        scanner.position());
                    return lexy::scan_failed;
                }
                else
                {
                    lexy::scan_result<lexy::lexeme<Reader>> scan_result;
                    scanner.parse(scan_result, everything_except_doublequote);
                    result.append(scan_result.value().begin(),
                        scan_result.value().end());
                }
            }

            while (scanner.discard(lexy::dsl::token(separator)))
                ; // skip whitespace

            char escape_character = '\\';
            if (scanner.branch(
                    lexy::dsl::ascii::case_folding(lexy::dsl::lit<"uescape">)))
            {
                while (scanner.discard(lexy::dsl::token(separator)))
                    ; // skip whitespace
                scanner.parse(lexy::dsl::apostrophe);
                if (!scanner)
                {
                    return lexy::scan_failed;
                }

                lexy::scan_result<lexy::lexeme<Reader>> scan_result;
                scanner.parse(scan_result, unicode_escape_character);
                escape_character =
                    scan_result
                        .value()[0]; // TODO-JK: Fail on multiple characters

                scanner.parse(lexy::dsl::apostrophe);
                if (!scanner)
                {
                    return lexy::scan_failed;
                }
            }

            return ast::unicode_delimited_identifier{result, escape_character};
        }
    };
} // namespace melinda::mdbsql::parser

#endif
