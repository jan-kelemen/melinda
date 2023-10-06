#ifndef MELINDA_MDBSQL_AST_TABLE_ELEMENTS_INCLUDED
#define MELINDA_MDBSQL_AST_TABLE_ELEMENTS_INCLUDED

#include <vector>

#include <mdbsql_ast_column_definition.h>

namespace melinda::mdbsql::ast
{
    struct [[nodiscard]] table_elements final
    {
        std::vector<column_definition> elements;

        bool operator==(table_elements const&) const = default;
    };
} // namespace melinda::mdbsql::ast

#endif
