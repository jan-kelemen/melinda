#ifndef MELINDA_MQLPRS_PARSER_INCLUDED
#define MELINDA_MQLPRS_PARSER_INCLUDED

#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <lexy/action/parse.hpp>
#include <lexy/callback/container.hpp>
#include <lexy/encoding.hpp>
#include <lexy/input/string_input.hpp>
#include <tl/expected.hpp>

#include <mqlprs_common.h>
#include <mqlprs_parse_error.h>

namespace melinda::mqlprs
{
    template<typename T>
    using parse_result = tl::expected<T, parse_error>;

    template<typename T, typename P = parser_for<T>::type>
    parse_result<T> parse(std::string_view query)
    {
        lexy::string_input<lexy::utf8_char_encoding> const range{query};

        auto result{lexy::parse<P>(range,
            lexy::collect<std::vector<parse_error_detail>>(error_callback{}))};

        if (result.is_success() && result.has_value())
        {
            return std::move(result).value();
        }
        else if (result.is_success())
        {
            std::vector<parse_error_detail> details{
                {0, 0, "Parsing produced no value", parse_error_kind::generic}};
            return parse_result<T>{tl::unexpect, std::move(details)};
        }
        else
        {
            return parse_result<T>{tl::unexpect, std::move(result).errors()};
        }
    }
} // namespace melinda::mqlprs

#endif
