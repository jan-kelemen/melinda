#ifndef MELINDA_MQLPRS_COMMON_INCLUDED
#define MELINDA_MQLPRS_COMMON_INCLUDED

#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include <lexy/dsl/ascii.hpp>
#include <lexy/dsl/branch.hpp>
#include <lexy/dsl/char_class.hpp>
#include <lexy/dsl/choice.hpp>
#include <lexy/dsl/code_point.hpp>
#include <lexy/dsl/digit.hpp>
#include <lexy/dsl/identifier.hpp>
#include <lexy/dsl/integer.hpp> // IWYU pragma: keep
#include <lexy/dsl/literal.hpp>
#include <lexy/dsl/newline.hpp>
#include <lexy/dsl/option.hpp> // IWYU pragma: keep
#include <lexy/dsl/scan.hpp>
#include <lexy/dsl/sign.hpp> // IWYU pragma: keep
#include <lexy/dsl/symbol.hpp>
#include <lexy/dsl/unicode.hpp>
#include <lexy/dsl/until.hpp>
#include <lexy/grammar.hpp>
#include <lexy/lexeme.hpp>

#include <mblcxx_always_false.h> // IWYU pragma: keep
#include <mqlast_common.h>

namespace melinda::mqlprs
{
    constexpr auto separator = LEXY_LIT("/*") >>
            lexy::dsl::until(LEXY_LIT("*/")) |
        LEXY_LIT("--") >> lexy::dsl::until(lexy::dsl::newline) |
        lexy::dsl::unicode::space;

    constexpr auto kw = lexy::dsl::identifier(lexy::dsl::ascii::alpha,
        lexy::dsl::ascii::alpha_underscore / LEXY_LIT("-"));

    template<typename Reader>
    [[nodiscard]] constexpr auto to_string_view(lexy::lexeme<Reader>&& l)
        -> std::basic_string_view<typename lexy::lexeme<Reader>::char_type>
    {
        return {l.begin(), l.end()};
    }

    [[nodiscard]] constexpr std::optional<mqlast::sign> to_ast_sign(
        auto&& parsed_value)
    {
        using T = std::decay_t<decltype(parsed_value)>;
        if constexpr (std::is_same_v<lexy::plus_sign, T>)
        {
            return mqlast::sign::plus;
        }
        else if constexpr (std::is_same_v<lexy::minus_sign, T>)
        {
            return mqlast::sign::minus;
        }
        else if constexpr (std::is_same_v<lexy::nullopt, T>)
        {
            return std::nullopt;
        }
        else
        {
            static_assert(mblcxx::always_false_v<T>, "Unrecognized sign type");
        }
    }

    constexpr auto sign = lexy::symbol_table<mqlast::sign>.map<LEXY_SYMBOL("+")>(mqlast::sign::plus).map<LEXY_SYMBOL("-")>(mqlast::sign::minus);

    struct [[nodiscard]] escaped_unicode_literal final
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
                        auto escape_value = scanner.template integer<int>(
                            unicode_6_digit_escape_value);
                        if (!escape_value)
                        {
                            return lexy::scan_failed;
                        }
                        rv += static_cast<char>(
                            escape_value.value()); // TODO-JK: handle values
                                                   // larger than char correctly
                    }
                    else if (scanner.peek(unicode_4_digit_escape_value))
                    {
                        auto escape_value = scanner.template integer<int>(
                            unicode_4_digit_escape_value);
                        if (!escape_value)
                        {
                            return lexy::scan_failed;
                        }
                        rv += static_cast<char>(
                            escape_value.value()); // TODO-JK: handle values
                                                   // larger than char correctly
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

    template<typename T>
    struct parser_for; // IWYU pragma: keep
} // namespace melinda::mqlprs

#endif
