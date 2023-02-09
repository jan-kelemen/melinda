#ifndef MELINDA_MDBSQL_PARSER_INCLUDED
#define MELINDA_MDBSQL_PARSER_INCLUDED

#include <optional>
#include <string_view>

#include <mdbsql_ast.h>

namespace melinda::mdbsql
{
    std::optional<ast::sql_executable_statement> parse(
        std::string_view statement);
} // namespace melinda::mdbsql
#endif
