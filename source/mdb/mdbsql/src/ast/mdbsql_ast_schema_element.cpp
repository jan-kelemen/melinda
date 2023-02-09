#include <mdbsql_ast_schema_element.h>

std::string melinda::mdbsql::ast::to_string(
    melinda::mdbsql::ast::schema_element const& value)
{
    return std::visit([](auto&& v) { return to_string(v); }, value);
}
