#ifndef MELINDA_MQLPRS_DATETIME_LITERAL_INCLUDED
#define MELINDA_MQLPRS_DATETIME_LITERAL_INCLUDED

#include <cstdint>
#include <utility>

#include <lexy/callback/adapter.hpp>
#include <lexy/callback/forward.hpp>
#include <lexy/callback/object.hpp>
#include <lexy/dsl/ascii.hpp>
#include <lexy/dsl/branch.hpp>
#include <lexy/dsl/case_folding.hpp>
#include <lexy/dsl/choice.hpp>
#include <lexy/dsl/identifier.hpp>
#include <lexy/dsl/integer.hpp>
#include <lexy/dsl/option.hpp>
#include <lexy/dsl/peek.hpp>
#include <lexy/dsl/production.hpp>
#include <lexy/dsl/punctuator.hpp>
#include <lexy/dsl/sequence.hpp>
#include <lexy/dsl/symbol.hpp>
#include <lexy/dsl/whitespace.hpp>

#include <mqlast_datetime_literal.h>

#include <mqlprs_common.h>
#include <mqlprs_reserved_word.h>

namespace melinda::mqlprs
{
    template<typename T>
    struct parser_for;
} // namespace melinda::mqlprs

namespace melinda::mqlprs
{
    struct [[nodiscard]] date_literal final
    {
        struct [[nodiscard]] unquoted final
        {
            static constexpr auto rule = lexy::dsl::integer<uint8_t> +
                lexy::dsl::hyphen + lexy::dsl::integer<uint8_t> +
                lexy::dsl::hyphen + lexy::dsl::integer<int64_t>;

            static constexpr auto value = lexy::construct<mqlast::date_literal>;
        };

        static constexpr auto whitespace = separator;

        static constexpr auto rule = kw_date >>
            lexy::dsl::no_whitespace(lexy::dsl::apostrophe +
                lexy::dsl::p<unquoted> + lexy::dsl::apostrophe);

        static constexpr auto value = lexy::forward<mqlast::date_literal>;
    };

    template<>
    struct parser_for<mqlast::date_literal>
    {
        using value_type = mqlast::date_literal;

        using type = date_literal;
    };

    struct [[nodiscard]] time_literal final
    {
        struct [[nodiscard]] time_zone_literal final
        {
            static constexpr auto rule =
                lexy::dsl::symbol<sign> >> lexy::dsl::integer<uint8_t> +
                    lexy::dsl::colon + lexy::dsl::integer<uint8_t>;

            static constexpr auto value =
                lexy::callback<mqlast::time_zone_literal>(
                    [](auto&& sign, auto&& hours, auto&& minutes)
                    {
                        return mqlast::time_zone_literal{std::move(sign),
                            std::move(hours),
                            std::move(minutes)};
                    });
        };

        struct [[nodiscard]] unquoted final
        {
            static constexpr auto rule = lexy::dsl::integer<uint8_t> +
                lexy::dsl::colon + lexy::dsl::integer<uint8_t> +
                lexy::dsl::colon + lexy::dsl::integer<uint8_t> +
                lexy::dsl::opt(lexy::dsl::period >>
                    lexy::dsl::opt(lexy::dsl::integer<uint32_t>)) +
                lexy::dsl::opt(lexy::dsl::p<time_zone_literal>);

            static constexpr auto value = lexy::callback<mqlast::time_literal>(
                [](auto&& hours, auto&& minutes, auto&& seconds)
                {
                    return mqlast::time_literal{std::move(hours),
                        std::move(minutes),
                        std::move(seconds)};
                },
                [](auto&& hours,
                    auto&& minutes,
                    auto&& seconds,
                    lexy::nullopt,
                    lexy::nullopt)
                {
                    return mqlast::time_literal{std::move(hours),
                        std::move(minutes),
                        std::move(seconds)};
                },
                [](auto&& hours,
                    auto&& minutes,
                    auto&& seconds,
                    auto&& fraction,
                    lexy::nullopt)
                {
                    return mqlast::time_literal{std::move(hours),
                        std::move(minutes),
                        std::move(seconds),
                        std::move(fraction)};
                },
                [](auto&& hours,
                    auto&& minutes,
                    auto&& seconds,
                    lexy::nullopt,
                    mqlast::time_zone_literal&& tz)
                {
                    return mqlast::time_literal{std::move(hours),
                        std::move(minutes),
                        std::move(seconds),
                        uint32_t{0},
                        std::move(tz)};
                },
                [](auto&& hours,
                    auto&& minutes,
                    auto&& seconds,
                    auto&& fraction,
                    mqlast::time_zone_literal&& tz)
                {
                    return mqlast::time_literal{std::move(hours),
                        std::move(minutes),
                        std::move(seconds),
                        std::move(fraction),
                        std::move(tz)};
                },
                [](auto&& hours,
                    auto&& minutes,
                    auto&& seconds,
                    mqlast::time_zone_literal&& tz)
                {
                    return mqlast::time_literal{std::move(hours),
                        std::move(minutes),
                        std::move(seconds),
                        uint32_t{0},
                        std::move(tz)};
                });
        };

        static constexpr auto whitespace = separator;

        static constexpr auto rule = kw_time >>
            lexy::dsl::no_whitespace(lexy::dsl::apostrophe +
                lexy::dsl::p<unquoted> + lexy::dsl::apostrophe);

        static constexpr auto value = lexy::forward<mqlast::time_literal>;
    };

    template<>
    struct parser_for<mqlast::time_literal>
    {
        using value_type = mqlast::time_literal;

        using type = time_literal;
    };

    struct [[nodiscard]] timestamp_literal final
    {
        static constexpr auto whitespace = separator;

        static constexpr auto rule = kw_timestamp >>
            lexy::dsl::no_whitespace(lexy::dsl::apostrophe +
                lexy::dsl::p<date_literal::unquoted> + lexy::dsl::ascii::space +
                lexy::dsl::p<time_literal::unquoted> + lexy::dsl::apostrophe);

        static constexpr auto value =
            lexy::construct<mqlast::timestamp_literal>;
    };

    template<>
    struct parser_for<mqlast::timestamp_literal>
    {
        using value_type = mqlast::timestamp_literal;

        using type = timestamp_literal;
    };

    struct [[nodiscard]] datetime_literal final
    {
        // TODO-JK: Why these rules are not recognized as branches by
        // themselves?
        static constexpr auto rule = lexy::dsl::peek(kw_timestamp) >>
                lexy::dsl::p<timestamp_literal> |
            lexy::dsl::peek(kw_time) >> lexy::dsl::p<time_literal> |
            lexy::dsl::peek(kw_date) >> lexy::dsl::p<date_literal>;

        static constexpr auto value = lexy::construct<mqlast::datetime_literal>;
    };

    template<>
    struct parser_for<mqlast::datetime_literal>
    {
        using value_type = mqlast::datetime_literal;

        using type = datetime_literal;
    };
} // namespace melinda::mqlprs
#endif
