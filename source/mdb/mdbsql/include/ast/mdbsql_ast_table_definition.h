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
        identifier name;
        table_contents_source table_contents;

        bool operator==(table_definition const&) const = default;
    };

    std::string to_string(table_definition const& value);
} // namespace melinda::mdbsql::ast

template<>
struct fmt::formatter<melinda::mdbsql::ast::table_definition>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mdbsql::ast::table_definition const& value,
        FormatContext& ctx) const
    {
        return format_to(ctx.out(), "{}", to_string(value));
    }
};

#endif
