#ifndef MELINDA_MDBSQL_AST_COLUMN_DEFINITION_INCLUDED
#define MELINDA_MDBSQL_AST_COLUMN_DEFINITION_INCLUDED

#include <string>

#include <fmt/format.h>

#include <mdbsql_ast_identifier.h>

namespace melinda::mdbsql::ast
{
    struct [[nodiscard]] column_definition final
    {
        identifier name;
        std::string data_type;

        bool operator==(column_definition const&) const = default;
    };

    std::string to_string(column_definition const& value);
} // namespace melinda::mdbsql::ast

template<>
struct fmt::formatter<melinda::mdbsql::ast::column_definition>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mdbsql::ast::column_definition const& value,
        FormatContext& ctx) const
    {
        return format_to(ctx.out(), "{}", to_string(value));
    }
};

#endif
