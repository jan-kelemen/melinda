#include <mdbsql_ast_sql_executable_statement.h>

std::string melinda::mdbsql::ast::to_string(
    melinda::mdbsql::ast::sql_executable_statement const& value)
{
    return std::visit([](auto&& v) { return to_string(v); }, value);
}
