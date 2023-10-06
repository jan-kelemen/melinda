#ifndef MELINDA_MDBSQL_AST_TABLE_DEFINITION_INCLUDED
#define MELINDA_MDBSQL_AST_TABLE_DEFINITION_INCLUDED

#include <optional>
#include <string>
#include <variant>

#include <fmt/format.h>

#include <mdbsql_ast_table_elements.h>

namespace melinda::mdbsql::ast
{
    struct [[nodiscard]] table_definition final
    {
        using table_contents_source = std::variant<table_elements>;

        std::optional<std::string> scope;
        multipart_identifier name;
        table_contents_source table_contents;

        bool operator==(table_definition const&) const = default;
    };
} // namespace melinda::mdbsql::ast

#endif
