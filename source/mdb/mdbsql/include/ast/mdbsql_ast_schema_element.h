#ifndef MELINDA_MDBSQL_AST_SCHEMA_ELEMENT_INCLUDED
#define MELINDA_MDBSQL_AST_SCHEMA_ELEMENT_INCLUDED

#include <string>
#include <variant>

#include <mdbsql_ast_table_definition.h>

namespace melinda::mdbsql::ast
{
    using schema_element = std::variant<table_definition>;
} // namespace melinda::mdbsql::ast

#endif
