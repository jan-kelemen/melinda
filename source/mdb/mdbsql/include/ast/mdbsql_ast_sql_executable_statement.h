#ifndef MELINDA_MDBSQL_AST_SQL_EXECUTABLE_STATEMENT_INCLUDED
#define MELINDA_MDBSQL_AST_SQL_EXECUTABLE_STATEMENT_INCLUDED

#include <variant>

#include <mdbsql_ast_sql_schema_statement.h>

namespace melinda::mdbsql::ast
{
    using sql_executable_statement = std::variant<sql_schema_statement>;
} // namespace melinda::mdbsql::ast

#endif
