#include <mdbsql_ast_column_definition.h>

std::string melinda::mdbsql::ast::to_string(
    melinda::mdbsql::ast::column_definition const& value)
{
    return fmt::format("{} {}", value.name, value.data_type);
}
