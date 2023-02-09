#include <mdbsql_ast_identifier.h>

#include <fmt/ranges.h>

std::string melinda::mdbsql::ast::to_string(
    melinda::mdbsql::ast::identifier const& value)
{
    return fmt::format("{}", fmt::join(value.parts, "."));
}
