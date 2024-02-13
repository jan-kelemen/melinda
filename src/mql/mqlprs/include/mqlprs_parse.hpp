#ifndef MELINDA_MQLPRS_PARSE_INCLUDED
#define MELINDA_MQLPRS_PARSE_INCLUDED

#include <mblcxx_result.hpp>

#include <mqlprs_parse_errors.hpp>

#include <lexy/action/parse_as_tree.hpp>
#include <lexy/dsl.hpp>
#include <lexy/encoding.hpp>
#include <lexy/input/string_input.hpp>
#include <lexy/parse_tree.hpp>

#include <string_view>

namespace melinda::mqlprs
{
    constexpr auto separator = LEXY_LIT("/*") >>
            lexy::dsl::until(LEXY_LIT("*/")) |
        LEXY_LIT("--") >> lexy::dsl::until(lexy::dsl::newline).or_eof() |
        lexy::dsl::unicode::space;

    constexpr auto keyword = lexy::dsl::identifier(lexy::dsl::ascii::alpha,
        lexy::dsl::ascii::alpha_underscore / LEXY_LIT("-"));

    constexpr auto kw_create =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD("create", keyword));

    constexpr auto kw_schema =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD("schema", keyword));

    constexpr auto reserved_words =
        lexy::dsl::literal_set(kw_create, kw_schema);

    struct identifier final
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

            return lexy::dsl::identifier(identifier_start, identifier_part)
                .reserve(reserved_words);
        }();
    };

    struct create_statement final
    {
        static constexpr auto rule =
            kw_create >> (kw_schema + lexy::dsl::p<identifier>);
    };

    struct statement final : lexy::transparent_production
    {
        static constexpr auto rule = lexy::dsl::p<create_statement>;
    };

    struct root final
    {
        static constexpr auto whitespace = separator;

        static constexpr auto rule =
            lexy::dsl::list(lexy::dsl::p<statement>,
                lexy::dsl::trailing_sep(lexy::dsl::semicolon)) +
            lexy::dsl::eof;
    };

    template<typename Production = root>
    [[nodiscard]] mblcxx::result<
        lexy::parse_tree_for<lexy::string_input<lexy::utf8_char_encoding>>,
        parse_errors>
    parse(std::string_view query)
    {
        lexy::string_input<lexy::utf8_char_encoding> const input{query.data(),
            query.size()};

        lexy::parse_tree_for<decltype(input)> tree;
        if (auto result{lexy::parse_as_tree<Production>(tree,
                input,
                lexy::collect<parse_errors>(error_callback{}))};
            !result)
        {
            return mblcxx::error<parse_errors>{std::move(result).errors()};
        }

        return tree;
    }
} // namespace melinda::mqlprs

#endif
