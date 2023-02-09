#ifndef MELINDA_MDBSQL_AST_SQL_EXECUTABLE_STATEMENT_INCLUDED
#define MELINDA_MDBSQL_AST_SQL_EXECUTABLE_STATEMENT_INCLUDED

#include <variant>

#include <mdbsql_ast_sql_schema_statement.h>

namespace melinda::mdbsql::ast
{
    using sql_executable_statement = std::variant<sql_schema_statement>;

    std::string to_string(sql_executable_statement const& value);
} // namespace melinda::mdbsql::ast

template<>
struct fmt::formatter<melinda::mdbsql::ast::sql_executable_statement>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mdbsql::ast::sql_executable_statement const& value,
        FormatContext& ctx) const
    {
        return format_to(ctx.out(), "{}", to_string(value));
    }
};
#endif
