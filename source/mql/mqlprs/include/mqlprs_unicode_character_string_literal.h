#ifndef MELINDA_MQLPRS_UNICODE_CHARACTER_STRING_LITERAL
#define MELINDA_MQLPRS_UNICODE_CHARACTER_STRING_LITERAL

#include <lexy/action/parse.hpp>
#include <lexy/dsl/literal.hpp>

#include <lexy/dsl/punctuator.hpp>
#include <lexy/dsl/scan.hpp>

#include <mqlast_unicode_character_string_literal.h>
#include <mqlprs_common.h>
#include <mqlprs_identifier.h>

namespace melinda::mqlprs
{
    template<typename T>
    struct parser_for;
} // namespace melinda::mqlprs

namespace melinda::mqlprs
{
    struct [[nodiscard]] unicode_character_string_literal final
        : lexy::scan_production<mqlast::unicode_character_string_literal>
    {
        static constexpr auto introductor =
            lexy::dsl::peek(lexy::dsl::opt(lexy::dsl::lit_c<'_'> >>
                                lexy::dsl::p<multipart_identifier>) +
                (lexy::dsl::lit_c<'u'> /
                    lexy::dsl::lit_c<'U'>) +lexy::dsl::apostrophe);

        template<typename Context, typename Reader>
        static constexpr scan_result scan(
            lexy::rule_scanner<Context, Reader>& scanner)
        {
            auto unicode_escape_character =
                lexy::dsl::capture(-(lexy::dsl::digit<lexy::dsl::hex> /
                    lexy::dsl::hyphen / lexy::dsl::apostrophe /
                    lexy::dsl::lit_c<'"'> / lexy::dsl::unicode::space));

            std::optional<mqlast::multipart_identifier> character_set;
            if (scanner.branch(lexy::dsl::lit_c<'_'>))
            {
                lexy::scan_result<mqlast::multipart_identifier> scan_result =
                    scanner.parse(multipart_identifier{});
                character_set = std::move(scan_result).value();

                while (scanner.discard(lexy::dsl::token(separator)))
                    ; // skip whitespace
            }

            auto everything_except_quote =
                lexy::dsl::capture(-lexy::dsl::apostrophe);

            scanner.parse(lexy::dsl::lit_c<'u'> / lexy::dsl::lit_c<'U'> +
                lexy::dsl::apostrophe);
            if (!scanner)
            {
                return lexy::scan_failed;
            }

            std::string intermediate_result;

            auto literal_start = scanner.position();
            auto found_closing_quote{false};
            while (!found_closing_quote)
            {
                if (scanner.branch(LEXY_LIT("''")))
                {
                    intermediate_result += '\'';
                }
                else if (scanner.branch(lexy::dsl::apostrophe))
                {
                    while (scanner.discard(lexy::dsl::token(separator)))
                        ; // skip whitespace

                    found_closing_quote =
                        !scanner.branch(lexy::dsl::apostrophe);
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
                    scanner.parse(scan_result, everything_except_quote);
                    intermediate_result +=
                        to_string_view(std::move(scan_result).value());
                }
            }

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
            auto result = lexy::parse<escaped_unicode_literal>(range,
                escape_character,
                lexy::collect<std::vector<parse_error_detail>>(
                    error_callback{}));
            if (!result)
            {
                scanner.error("Unparsable string", literal_start);
                return lexy::scan_failed;
            }

            return mqlast::unicode_character_string_literal{
                std::move(character_set),
                std::move(result).value(),
                escape_character};
        }
    };

    template<>
    struct parser_for<mqlast::unicode_character_string_literal>
    {
        using value_type = mqlast::unicode_character_string_literal;

        using type = unicode_character_string_literal;
    };
} // namespace melinda::mqlprs
#endif
