#ifndef MELINDA_MQLPRS_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_IDENTIFIER_INCLUDED

#include <vector>

#include <lexy/callback/adapter.hpp>
#include <lexy/callback/composition.hpp>
#include <lexy/callback/container.hpp>
#include <lexy/callback/object.hpp>
#include <lexy/dsl/list.hpp>
#include <lexy/dsl/production.hpp>

#include <mqlprs_ast_identifier.h> // IWYU pragma: export
#include <mqlprs_delimited_identifier.h>
#include <mqlprs_regular_identifier.h>
#include <mqlprs_unicode_delimited_identifier.h>

namespace melinda::mqlprs
{
    struct [[nodiscard]] identifier final
    {
        static constexpr auto rule =
            lexy::dsl::p<unicode_delimited_identifier> |
            lexy::dsl::p<delimited_identifier> |
            lexy::dsl::p<regular_identifier>;

        static constexpr auto value = lexy::construct<ast::identifier>;
    };

    template<>
    struct parser_for<ast::identifier>
    {
        using value_type = ast::identifier;

        using type = identifier;
    };

    struct [[nodiscard]] multipart_identifier final
    {
        static constexpr auto rule = lexy::dsl::list(lexy::dsl::p<identifier>,
            lexy::dsl::sep(lexy::dsl::period));

        static constexpr auto
            value = lexy::as_list<std::vector<ast::identifier>> >>
            lexy::callback<ast::multipart_identifier>([](auto&& parts)
                { return ast::multipart_identifier{std::move(parts)}; });
    };

    template<>
    struct parser_for<ast::multipart_identifier>
    {
        using value_type = ast::multipart_identifier;

        using type = multipart_identifier;
    };
} // namespace melinda::mqlprs

#endif
