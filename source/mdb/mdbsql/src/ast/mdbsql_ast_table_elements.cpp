#include <mdbsql_ast_table_elements.h>

#include <fmt/ranges.h>

std::string melinda::mdbsql::ast::to_string(
    melinda::mdbsql::ast::table_elements const& value)
{
    return fmt::format("{}", fmt::join(value.elements, ", "));
}
