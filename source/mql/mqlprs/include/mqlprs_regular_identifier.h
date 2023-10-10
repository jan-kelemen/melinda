#ifndef MELINDA_MQLPRS_REGULAR_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_REGULAR_IDENTIFIER_INCLUDED

#include <string_view>

#include <lexy/callback/adapter.hpp>
#include <lexy/code_point.hpp>
#include <lexy/dsl/char_class.hpp>
#include <lexy/dsl/code_point.hpp>
#include <lexy/dsl/identifier.hpp>
#include <lexy/dsl/literal.hpp>

#include <mqlprs_ast_regular_identifier.h> // IWYU pragma: keep
#include <mqlprs_reserved_word.h>

namespace melinda::mqlprs
{
    template<typename T>
    struct parser_for;
} // namespace melinda::mqlprs

namespace melinda::mqlprs
{
    struct [[nodiscard]] regular_identifier final
    {
        static constexpr auto rule = []
        {
            auto identifier_start =
                lexy::dsl::code_point.general_category<lexy::code_point::Lu>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Ll>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Lt>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Lm>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Lo>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Nl>();

            auto identifier_extend = lexy::dsl::lit_cp<0x00B7> /
                lexy::dsl::code_point.general_category<lexy::code_point::Mn>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Mc>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Nd>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Pc>() /
                lexy::dsl::code_point.general_category<lexy::code_point::Cf>();

            auto identifier_part = identifier_start / identifier_extend;

            // TODO-JK: Limit the number of <identifier part> to < 128
            return lexy::dsl::identifier(identifier_start, identifier_part)
                .reserve(reserved_set1)
                .reserve(reserved_set2);
        }();

        static constexpr auto value = lexy::callback<ast::regular_identifier>(
            [](auto lexeme)
            {
                return ast::regular_identifier{
                    std::string_view{lexeme.begin(), lexeme.end()}};
            });
    };

    template<>
    struct parser_for<ast::regular_identifier>
    {
        using value_type = ast::regular_identifier;

        using type = regular_identifier;
    };
} // namespace melinda::mqlprs

#endif
