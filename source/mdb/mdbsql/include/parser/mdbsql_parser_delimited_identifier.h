#ifndef MELINDA_MDBSQL_PARSER_DELIMITED_IDENTIFIER_INCLUDED
#define MELINDA_MDBSQL_PARSER_DELIMITED_IDENTIFIER_INCLUDED

#include <lexy/callback/adapter.hpp>
#include <lexy/code_point.hpp>
#include <lexy/dsl/capture.hpp>
#include <lexy/dsl/code_point.hpp>
#include <lexy/dsl/literal.hpp>
#include <lexy/dsl/scan.hpp>
#include <lexy/grammar.hpp>
#include <lexy/lexeme.hpp>

#include <mdbsql_ast_delimited_identifier.h>

namespace melinda::mdbsql::parser
{
    struct [[nodiscard]] delimited_identifier final
        : lexy::scan_production<ast::delimited_identifier>
        , lexy::token_production
    {
        static constexpr auto rule = lexy::dsl::lit_c<'"'> >> lexy::dsl::scan;

        template<typename Context, typename Reader>
        static constexpr scan_result scan(
            lexy::rule_scanner<Context, Reader>& scanner)
        {
            auto everything_except_doublequote =
                lexy::dsl::capture(-lexy::dsl::lit_c<'"'>);

            std::string result;

            auto found_closing_quote{false};
            while (!found_closing_quote)
            {
                if (scanner.branch(LEXY_LIT("\"\"")))
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

            return ast::delimited_identifier{std::move(result)};
        }
    };
} // namespace melinda::mdbsql::parser

#endif
