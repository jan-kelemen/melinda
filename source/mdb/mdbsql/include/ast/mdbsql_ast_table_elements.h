#ifndef MELINDA_MDBSQL_AST_TABLE_ELEMENTS_INCLUDED
#define MELINDA_MDBSQL_AST_TABLE_ELEMENTS_INCLUDED

#include <vector>

#include <fmt/format.h>

#include <mdbsql_ast_column_definition.h>

namespace melinda::mdbsql::ast
{
    struct [[nodiscard]] table_elements final
    {
        std::vector<column_definition> elements;

        bool operator==(table_elements const&) const = default;
    };

    std::string to_string(table_elements const& value);
} // namespace melinda::mdbsql::ast

template<>
struct fmt::formatter<melinda::mdbsql::ast::table_elements>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mdbsql::ast::table_elements const& value,
        FormatContext& ctx) const
    {
        return format_to(ctx.out(), "{}", to_string(value));
    }
};
#endif
