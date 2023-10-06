#ifndef MELINDA_MDBSQL_AST_COLUMN_DEFINITION_INCLUDED
#define MELINDA_MDBSQL_AST_COLUMN_DEFINITION_INCLUDED

#include <string>

#include <mdbsql_ast_identifier.h>

namespace melinda::mdbsql::ast
{
    struct [[nodiscard]] column_definition final
    {
        multipart_identifier name;
        std::string data_type;

        bool operator==(column_definition const&) const = default;
    };
} // namespace melinda::mdbsql::ast

#endif
