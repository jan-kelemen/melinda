#ifndef MELINDA_MQLPRS_CHARACTER_STRING_LITERAL
#define MELINDA_MQLPRS_CHARACTER_STRING_LITERAL

#include <charconv>

#include <lexy/dsl/digit.hpp>
#include <lexy/dsl/literal.hpp>

#include <lexy/dsl/punctuator.hpp>
#include <lexy/dsl/scan.hpp>

#include <lexy/error.hpp>
#include <mqlast_binary_string_literal.h>
#include <mqlprs_common.h>
#include <mqlprs_identifier.h>

namespace melinda::mqlprs
{
    template<typename T>
    struct parser_for;
} // namespace melinda::mqlprs

namespace melinda::mqlprs
{
    struct [[nodiscard]] binary_string_literal final
        : lexy::scan_production<mqlast::binary_string_literal>
    {
        static constexpr auto rule =
            (lexy::dsl::lit_c<'x'> / lexy::dsl::lit_c<'X'>) >> lexy::dsl::scan;

        template<typename Context, typename Reader>
        static constexpr scan_result scan(
            lexy::rule_scanner<Context, Reader>& scanner)
        {
            auto hex_digit =
                lexy::dsl::capture(lexy::dsl::digit<lexy::dsl::hex>);

            scanner.parse(lexy::dsl::apostrophe);
            if (!scanner)
            {
                return lexy::scan_failed;
            }

            std::string intermediate_result;

            auto found_closing_quote{false};
            while (!found_closing_quote)
            {
                while (scanner.discard(lexy::dsl::token(separator)))
                    ; // skip whitespace

                if (scanner.branch(LEXY_LIT("''")))
                {
                    ; // skip on consecutive literals
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
                    scanner.parse(scan_result, hex_digit);
                    intermediate_result +=
                        to_string_view(std::move(scan_result).value());
                }
            }

            if (intermediate_result.size() % 2)
            {
                scanner.fatal_error(lexy::expected_char_class{},
                    scanner.begin(),
                    scanner.position());
                return lexy::scan_failed;
            }

            std::vector<std::byte> result;
            for (size_t i{0}; i != intermediate_result.size(); i += 2)
            {
                auto begin{intermediate_result.data() + i};
                auto end{begin + 2};
                unsigned char v;
                std::from_chars(begin, end, v, 16);

                result.emplace_back(static_cast<std::byte>(v));
            }

            return mqlast::binary_string_literal{std::move(result)};
        }
    };

    template<>
    struct parser_for<mqlast::binary_string_literal>
    {
        using value_type = mqlast::binary_string_literal;

        using type = binary_string_literal;
    };
} // namespace melinda::mqlprs
#endif
