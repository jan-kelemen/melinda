#ifndef MELINDA_MQLPRS_CHARACTER_STRING_LITERAL
#define MELINDA_MQLPRS_CHARACTER_STRING_LITERAL

#include <lexy/dsl/literal.hpp>

#include <lexy/dsl/punctuator.hpp>
#include <lexy/dsl/scan.hpp>

#include <mqlast_character_string_literal.h>
#include <mqlprs_common.h>
#include <mqlprs_identifier.h>

namespace melinda::mqlprs
{
    template<typename T>
    struct parser_for;
} // namespace melinda::mqlprs

namespace melinda::mqlprs
{
    struct [[nodiscard]] character_string_literal final
        : lexy::scan_production<mqlast::character_string_literal>
    {
        static constexpr auto introductor =
            lexy::dsl::peek(lexy::dsl::opt(lexy::dsl::lit_c<'_'> >>
                                lexy::dsl::p<multipart_identifier>) +
                lexy::dsl::apostrophe);

        template<typename Context, typename Reader>
        static constexpr scan_result scan(
            lexy::rule_scanner<Context, Reader>& scanner)
        {
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

            scanner.parse(lexy::dsl::apostrophe);
            if (!scanner)
            {
                return lexy::scan_failed;
            }

            std::string result;

            auto found_closing_quote{false};
            while (!found_closing_quote)
            {
                if (scanner.branch(LEXY_LIT("''")))
                {
                    result += '\'';
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
                    result += to_string_view(std::move(scan_result).value());
                }
            }

            return mqlast::character_string_literal{std::move(character_set),
                std::move(result)};
        }
    };

    template<>
    struct parser_for<mqlast::character_string_literal>
    {
        using value_type = mqlast::character_string_literal;

        using type = character_string_literal;
    };
} // namespace melinda::mqlprs
#endif
