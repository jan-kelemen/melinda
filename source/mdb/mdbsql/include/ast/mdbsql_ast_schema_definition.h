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
        using character_set_or_path_spec =
            std::variant<identifier, std::vector<identifier>>;

        std::optional<identifier> schema_name;
        std::optional<std::string> authorization;
        std::optional<character_set_or_path_spec> character_set_or_path1;
        std::optional<character_set_or_path_spec> character_set_or_path2;
        std::optional<schema_element> elements;

        bool operator==(schema_definition const&) const = default;
    };

    std::string to_string(schema_definition const& value);
} // namespace melinda::mdbsql::ast

template<>
struct fmt::formatter<melinda::mdbsql::ast::schema_definition>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mdbsql::ast::schema_definition const& value,
        FormatContext& ctx) const
    {
        return format_to(ctx.out(), "{}", to_string(value));
    }
};
#endif
