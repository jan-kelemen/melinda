#ifndef MELINDA_MDBSQL_AST_SCHEMA_DEFINITION_INCLUDED
#define MELINDA_MDBSQL_AST_SCHEMA_DEFINITION_INCLUDED

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <mdbsql_ast_identifier.h>
#include <mdbsql_ast_schema_element.h>

namespace melinda::mdbsql::ast
{
    struct [[nodiscard]] schema_definition final
    {
        using character_set_or_path_spec = std::variant<multipart_identifier,
            std::vector<multipart_identifier>>;

        std::optional<multipart_identifier> schema_name;
        std::optional<identifier> authorization;
        std::optional<character_set_or_path_spec> character_set_or_path1;
        std::optional<character_set_or_path_spec> character_set_or_path2;
        std::optional<schema_element> elements;

        bool operator==(schema_definition const&) const = default;
    };
} // namespace melinda::mdbsql::ast

#endif
