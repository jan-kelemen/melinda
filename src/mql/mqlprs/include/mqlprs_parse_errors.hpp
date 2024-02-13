#ifndef MELINDA_MQLPRS_PARSE_ERRORS_INCLUDED
#define MELINDA_MQLPRS_PARSE_ERRORS_INCLUDED

#include <lexy/error.hpp>
#include <lexy/input_location.hpp>

#include <cstddef>
#include <string_view>
#include <vector>

namespace melinda::mqlprs
{
    enum class error_kind
    {
        expected_literal,
        expected_keyword,
        expected_char_class,
        generic,
    };

    struct parse_error final
    {
        std::size_t line_number;
        std::size_t column_number;
        std::string_view production;
        error_kind kind;
        std::string_view message;
    };

    struct error_callback final
    {
        using return_type = parse_error;

        template<typename Input, typename Tag>
        parse_error operator()(lexy::error_context<Input> const& context,
            lexy::error_for<Input, Tag> const& error) const
        {
            lexy::input_location const context_location{
                lexy::get_input_location(context.input(), context.position())};
            lexy::input_location const location{
                lexy::get_input_location(context.input(),
                    error.position(),
                    context_location.anchor())};

            std::size_t const line_number{location.line_nr() - 1};
            std::size_t const column_number{location.column_nr() - 1};

            if constexpr (std::is_same_v<Tag, lexy::expected_literal>)
            {
                return {line_number,
                    column_number,
                    context.production(),
                    error_kind::expected_literal,
                    error.string()};
            }
            else if constexpr (std::is_same_v<Tag, lexy::expected_keyword>)
            {
                return {line_number,
                    column_number,
                    context.production(),
                    error_kind::expected_keyword,
                    error.string()};
            }
            else if constexpr (std::is_same_v<Tag, lexy::expected_char_class>)
            {
                return {line_number,
                    column_number,
                    context.production(),
                    error_kind::expected_char_class,
                    error.name()};
            }
            else
            {
                return {line_number,
                    column_number,
                    context.production(),
                    error_kind::generic,
                    error.message()};
            }
        }
    };

    using parse_errors = std::vector<parse_error>;
} // namespace melinda::mqlprs

#endif
