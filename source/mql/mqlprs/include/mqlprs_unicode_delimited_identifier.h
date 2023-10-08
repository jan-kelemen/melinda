#ifndef MELINDA_MQLPRS_UNICODE_DELIMITED_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_UNICODE_DELIMITED_IDENTIFIER_INCLUDED

#include <lexy/action/parse.hpp>
#include <lexy/callback/adapter.hpp>
#include <lexy/callback/container.hpp>
#include <lexy/code_point.hpp>
#include <lexy/dsl/ascii.hpp>
#include <lexy/dsl/capture.hpp>
#include <lexy/dsl/case_folding.hpp>
#include <lexy/dsl/code_point.hpp>
#include <lexy/dsl/digit.hpp>
#include <lexy/dsl/integer.hpp>
#include <lexy/dsl/literal.hpp>
#include <lexy/dsl/peek.hpp>
#include <lexy/dsl/punctuator.hpp>
#include <lexy/dsl/scan.hpp>
#include <lexy/dsl/sign.hpp>
#include <lexy/dsl/token.hpp>
#include <lexy/dsl/unicode.hpp>
#include <lexy/encoding.hpp>
#include <lexy/grammar.hpp>
#include <lexy/input/string_input.hpp>
#include <lexy/lexeme.hpp>

#include <mqlprs_ast_unicode_delimited_identifier.h> // IWYU pragma: export
#include <mqlprs_common.h>
#include <mqlprs_parse_error.h>
#include <mqlprs_reserved_word.h>

namespace melinda::mqlprs
{
    struct [[nodiscard]] unicode_string_literal final
        : lexy::scan_production<std::string>
        , lexy::token_production
    {
        template<typename Context, typename Reader>
        static constexpr scan_result scan(
            lexy::rule_scanner<Context, Reader>& scanner,
            char escape_character)
        {
            auto unicode_4_digit_escape_value =
                lexy::dsl::n_digits<4, lexy::dsl::hex>;

            auto unicode_6_digit_escape_value =
                lexy::dsl::n_digits<6, lexy::dsl::hex>;

            std::string rv;

            bool pending_escape{false};
            while (!scanner.is_at_eof())
            {
                auto scan_result = scanner.capture(lexy::dsl::code_point);
                if (scan_result.value()[0] == escape_character)
                {
                    if (scanner.branch(lexy::dsl::lit_c<'+'>))
                    {
                        auto value = scanner.template integer<int>(
                            unicode_6_digit_escape_value);
                        if (!value)
                        {
                            return lexy::scan_failed;
                        }
                        rv += static_cast<char>(
                            value.value()); // TODO-JK: handle values larger
                                            // than char correctly
                    }
                    else if (scanner.peek(unicode_4_digit_escape_value))
                    {
                        auto value = scanner.template integer<int>(
                            unicode_4_digit_escape_value);
                        if (!value)
                        {
                            return lexy::scan_failed;
                        }
                        rv += static_cast<char>(
                            value.value()); // TODO-JK: handle values larger
                                            // than char correctly
                    }
                    else if (pending_escape)
                    {
                        rv += escape_character;
                        pending_escape = false;
                    }
                    else
                    {
                        pending_escape = true;
                    }
                }
                else if (pending_escape)
                {
                    scanner.error("Expected escape character",
                        scanner.position());
                    return lexy::scan_failed;
                }
                else
                {
                    rv += scan_result.value()[0];
                }
            }
            return rv;
        }
    };

    struct [[nodiscard]] unicode_delimited_identifier final
        : lexy::scan_production<ast::unicode_delimited_identifier>
        , lexy::token_production
    {
        static constexpr auto rule =
            lexy::dsl::ascii::case_folding(LEXY_LIT("u&\"")) >> lexy::dsl::scan;

        template<typename Context, typename Reader>
        static constexpr scan_result scan(
            lexy::rule_scanner<Context, Reader>& scanner)
        {
            auto everything_except_doublequote = -lexy::dsl::lit_c<'"'>;

            auto unicode_escape_character =
                lexy::dsl::capture(-(lexy::dsl::digit<lexy::dsl::hex> /
                    lexy::dsl::hyphen / lexy::dsl::apostrophe /
                    lexy::dsl::lit_c<'"'> / lexy::dsl::unicode::space));

            std::string intermediate_result;

            auto literal_start{scanner.position()};

            auto found_closing_quote{false};
            while (!found_closing_quote)
            {
                if (scanner.branch(LEXY_LIT("\"\"")))
                {
                    intermediate_result += '"';
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
                    auto scan_result =
                        scanner.capture(everything_except_doublequote);
                    intermediate_result.append(scan_result.value().begin(),
                        scan_result.value().end());
                }
            }

            while (scanner.discard(lexy::dsl::token(separator)))
                ; // skip whitespace

            char escape_character = '\\';
            if (scanner.branch(
                    lexy::dsl::ascii::case_folding(LEXY_LIT("uescape"))))
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

            lexy::string_input<lexy::utf8_char_encoding> const range{
                intermediate_result};
            auto result = lexy::parse<unicode_string_literal>(range,
                escape_character,
                lexy::collect<std::vector<parse_error_detail>>(
                    error_callback{}));
            if (!result)
            {
                scanner.error("Unparsable string", literal_start);
                return lexy::scan_failed;
            }

            return ast::unicode_delimited_identifier{std::move(result).value(),
                escape_character};
        }
    };

    template<>
    struct parser_for<ast::unicode_delimited_identifier>
    {
        using value_type = ast::unicode_delimited_identifier;

        using type = unicode_delimited_identifier;
    };
} // namespace melinda::mqlprs

#endif
