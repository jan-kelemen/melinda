#ifndef MELINDA_MDBSQL_AST_SQL_SCHEMA_ELEMENT_INCLUDED
#define MELINDA_MDBSQL_AST_SQL_SCHEMA_ELEMENT_INCLUDED

#include <variant>

#include <fmt/format.h>

#include <mdbsql_ast_schema_definition.h>
#include <mdbsql_ast_table_definition.h>

namespace melinda::mdbsql::ast
{
    using sql_schema_statement =
        std::variant<schema_definition, table_definition>;

    std::string to_string(sql_schema_statement const& value);
} // namespace melinda::mdbsql::ast

template<>
struct fmt::formatter<melinda::mdbsql::ast::sql_schema_statement>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mdbsql::ast::sql_schema_statement const& value,
        FormatContext& ctx) const
    {
        return format_to(ctx.out(), "{}", to_string(value));
    }
};
#endif
