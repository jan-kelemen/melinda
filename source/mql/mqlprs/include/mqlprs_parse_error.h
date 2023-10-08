#ifndef MELINDA_MQLPRS_PARSE_ERROR
#define MELINDA_MQLPRS_PARSE_ERROR

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

#include <lexy/error.hpp>
#include <lexy/input_location.hpp>

namespace melinda::mqlprs
{
    enum class [[nodiscard]] parse_error_kind : std::uint8_t
    {
        expected_literal,
        expected_keyword,
        expected_char_class,
        generic,
    };

    struct [[nodiscard]] parse_error_detail final
    {
        std::size_t line_number;
        std::size_t column_number;
        std::string_view production;
        parse_error_kind kind;
    };

    struct [[nodiscard]] parse_error final
    {
        std::vector<parse_error_detail> details;
    };

    struct error_callback
    {
        using return_type = parse_error_detail;

        template<typename Input, typename Tag>
        parse_error_detail operator()(lexy::error_context<Input> const& context,
            lexy::error_for<Input, Tag> const& error)
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
                    parse_error_kind::expected_literal};
            }
            else if constexpr (std::is_same_v<Tag, lexy::expected_keyword>)
            {
                return {line_number,
                    column_number,
                    context.production(),
                    parse_error_kind::expected_keyword};
            }
            else if constexpr (std::is_same_v<Tag, lexy::expected_char_class>)
            {
                return {line_number,
                    column_number,
                    context.production(),
                    parse_error_kind::expected_char_class};
            }
            else
            {
                return {line_number,
                    column_number,
                    context.production(),
                    parse_error_kind::generic};
            }
        }
    };
} // namespace melinda::mqlprs

#endif
