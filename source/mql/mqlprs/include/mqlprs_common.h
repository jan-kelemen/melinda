#ifndef MELINDA_MQLPRS_COMMON_INCLUDED
#define MELINDA_MQLPRS_COMMON_INCLUDED

#include <optional>
#include <string_view>
#include <type_traits>

#include <lexy/dsl/ascii.hpp>
#include <lexy/dsl/branch.hpp>
#include <lexy/dsl/char_class.hpp>
#include <lexy/dsl/choice.hpp>
#include <lexy/dsl/identifier.hpp>
#include <lexy/dsl/literal.hpp>
#include <lexy/dsl/newline.hpp>
#include <lexy/dsl/option.hpp> // IWYU pragma: keep
#include <lexy/dsl/sign.hpp> // IWYU pragma: keep
#include <lexy/dsl/unicode.hpp>
#include <lexy/dsl/until.hpp>
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

    template<typename T>
    struct parser_for; // IWYU pragma: keep
} // namespace melinda::mqlprs

#endif
