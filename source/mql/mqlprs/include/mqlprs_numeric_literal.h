#ifndef MELINDA_MQLPRS_NUMERIC_LITERAL_INCLUDED
#define MELINDA_MQLPRS_NUMERIC_LITERAL_INCLUDED

#include <optional>

#include <lexy/callback/adapter.hpp>
#include <lexy/callback/object.hpp>
#include <lexy/dsl/branch.hpp>
#include <lexy/dsl/capture.hpp>
#include <lexy/dsl/char_class.hpp>
#include <lexy/dsl/choice.hpp>
#include <lexy/dsl/digit.hpp>
#include <lexy/dsl/if.hpp>
#include <lexy/dsl/literal.hpp>
#include <lexy/dsl/option.hpp>
#include <lexy/dsl/peek.hpp>
#include <lexy/dsl/production.hpp>
#include <lexy/dsl/punctuator.hpp>
#include <lexy/dsl/sequence.hpp>
#include <lexy/dsl/sign.hpp>

#include <mqlprs_ast_numeric_literal.h>
#include <mqlprs_common.h>

namespace melinda::mqlprs
{
    template<typename T>
    struct parser_for;
} // namespace melinda::mqlprs

namespace melinda::mqlprs
{
    struct [[nodiscard]] exact_numeric_literal final
    {
        static constexpr auto rule = []()
        {
            auto unsigned_integer = lexy::dsl::capture(lexy::dsl::digits<>);

            return (unsigned_integer >> lexy::dsl::opt(lexy::dsl::period >>
                                            lexy::dsl::opt(unsigned_integer))) |
                (lexy::dsl::period >> (lexy::dsl::nullopt + unsigned_integer));
            ;
        }();

        static constexpr auto value =
            lexy::callback<ast::exact_numeric_literal>(
                [](lexy::nullopt, auto&& decimal)
                {
                    return ast::exact_numeric_literal(std::nullopt,
                        to_string_view(std::move(decimal)));
                },
                [](auto&& whole, lexy::nullopt) {
                    return ast::exact_numeric_literal(
                        to_string_view(std::move(whole)));
                },
                [](auto&& whole, auto&& decimal)
                {
                    return ast::exact_numeric_literal(
                        to_string_view(std::move(whole)),
                        to_string_view(std::move(decimal)));
                });
    };

    template<>
    struct parser_for<ast::exact_numeric_literal> final
    {
        using value_type = ast::exact_numeric_literal;

        using type = exact_numeric_literal;
    };

    struct [[nodiscard]] approximate_numeric_literal final
    {
        static constexpr auto rule = []()
        {
            auto unsigned_integer = lexy::dsl::capture(lexy::dsl::digits<>);

            return lexy::dsl::p<exact_numeric_literal> +
                (lexy::dsl::lit_c<'e'> /
                    lexy::dsl::lit_c<'E'>) +lexy::dsl::sign +
                unsigned_integer;
        }();

        static constexpr auto value =
            lexy::callback<ast::approximate_numeric_literal>(
                [](auto&& mantissa, auto&& sign, auto&& exponent)
                {
                    return ast::approximate_numeric_literal(std::move(mantissa),
                        to_ast_sign(sign),
                        to_string_view(std::move(exponent)));
                },
                [](auto&& mantissa, auto&& exponent)
                {
                    return ast::approximate_numeric_literal(std::move(mantissa),
                        std::nullopt,
                        to_string_view(std::move(exponent)));
                });
    };

    template<>
    struct parser_for<ast::approximate_numeric_literal>
    {
        using value_type = ast::approximate_numeric_literal;

        using type = approximate_numeric_literal;
    };

    struct [[nodiscard]] unsigned_numeric_literal final
    {
        static constexpr auto rule = []()
        {
            auto condition =
                lexy::dsl::peek(lexy::dsl::p<approximate_numeric_literal>);

            return (condition >> lexy::dsl::p<approximate_numeric_literal>) |
                lexy::dsl::p<exact_numeric_literal>;
        }();

        static constexpr auto value =
            lexy::construct<ast::unsigned_numeric_literal>;
    };

    template<>
    struct parser_for<ast::unsigned_numeric_literal>
    {
        using value_type = ast::unsigned_numeric_literal;

        using type = unsigned_numeric_literal;
    };

    struct [[nodiscard]] signed_numeric_literal final
    {
        static constexpr auto rule =
            lexy::dsl::sign + lexy::dsl::inline_<unsigned_numeric_literal>;

        static constexpr auto value =
            lexy::callback<ast::signed_numeric_literal>(
                [](auto&& sign, auto&& literal) {
                    return ast::signed_numeric_literal(to_ast_sign(sign),
                        std::move(literal));
                },
                lexy::construct<ast::signed_numeric_literal>);
    };

    template<>
    struct parser_for<ast::signed_numeric_literal>
    {
        using value_type = ast::signed_numeric_literal;

        using type = signed_numeric_literal;
    };
} // namespace melinda::mqlprs
#endif
