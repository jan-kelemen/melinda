#ifndef MELINDA_MDBSQL_PARSER_INCLUDED
#define MELINDA_MDBSQL_PARSER_INCLUDED

#include <cstdint>
#include <optional>
#include <string_view>

#include <mblcxx_result.h>

#include <mdbsql_ast.h>

namespace melinda::mdbsql
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

    using parse_result =
        mblcxx::result<ast::sql_executable_statement, parse_error>;

    parse_result parse(std::string_view statement);
} // namespace melinda::mdbsql
#endif
