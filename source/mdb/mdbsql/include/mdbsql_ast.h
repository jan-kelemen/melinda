#ifndef MELINDA_MDBSQL_AST_INCLUDED
#define MELINDA_MDBSQL_AST_INCLUDED

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace melinda::mdbsql::ast
{
    struct identifier
    {
        std::vector<std::string> parts;
    };

    struct column_definition
    {
        identifier name;
        std::string data_type;
    };

    struct table_elements
    {
        std::vector<column_definition> elements;
    };

    struct table_definition
    {
        using table_contents_source = std::variant<table_elements>;

        std::optional<std::string> scope;
        identifier name;
        table_contents_source table_contents;
    };

    using schema_element = std::variant<table_definition>;

    struct schema_definition
    {
        using character_set_or_path_spec =
            std::variant<identifier, std::vector<identifier>>;

        std::optional<identifier> schema_name;
        std::optional<std::string> authorization;
        std::optional<character_set_or_path_spec> character_set_or_path1;
        std::optional<character_set_or_path_spec> character_set_or_path2;
        std::optional<schema_element> elements;
    };

    using sql_schema_statement = std::variant<schema_definition>;

    using sql_executable_statement = std::variant<sql_schema_statement>;

} // namespace melinda::mdbsql::ast

#endif
