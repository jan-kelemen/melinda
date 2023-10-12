#ifndef MELINDA_MQLPRS_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_IDENTIFIER_INCLUDED

#include <vector>

#include <lexy/callback/composition.hpp>
#include <lexy/callback/container.hpp>
#include <lexy/callback/object.hpp>
#include <lexy/dsl/choice.hpp>
#include <lexy/dsl/list.hpp>
#include <lexy/dsl/production.hpp>
#include <lexy/dsl/punctuator.hpp>
#include <lexy/dsl/separator.hpp>

#include <mqlast_identifier.h> // IWYU pragma: keep
#include <mqlprs_delimited_identifier.h> // IWYU pragma: keep
#include <mqlprs_regular_identifier.h> // IWYU pragma: keep
#include <mqlprs_unicode_delimited_identifier.h> // IWYU pragma: keep

namespace melinda::mqlprs
{
    template<typename T>
    struct parser_for;
} // namespace melinda::mqlprs

namespace melinda::mqlprs
{
    struct [[nodiscard]] identifier final
    {
        static constexpr auto rule =
            lexy::dsl::p<unicode_delimited_identifier> |
            lexy::dsl::p<delimited_identifier> |
            lexy::dsl::p<regular_identifier>;

        static constexpr auto value = lexy::construct<mqlast::identifier>;
    };

    template<>
    struct parser_for<mqlast::identifier>
    {
        using value_type = mqlast::identifier;

        using type = identifier;
    };

    struct [[nodiscard]] multipart_identifier final
    {
        static constexpr auto rule = lexy::dsl::list(lexy::dsl::p<identifier>,
            lexy::dsl::sep(lexy::dsl::period));

        static constexpr auto value =
            lexy::as_list<std::vector<mqlast::identifier>> >>
            lexy::construct<mqlast::multipart_identifier>;
    };

    template<>
    struct parser_for<mqlast::multipart_identifier>
    {
        using value_type = mqlast::multipart_identifier;

        using type = multipart_identifier;
    };
} // namespace melinda::mqlprs

#endif
