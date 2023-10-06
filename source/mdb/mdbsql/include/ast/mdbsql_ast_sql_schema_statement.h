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
} // namespace melinda::mdbsql::ast

#endif
