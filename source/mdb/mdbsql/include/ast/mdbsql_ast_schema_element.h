#ifndef MELINDA_MDBSQL_AST_SCHEMA_ELEMENT_INCLUDED
#define MELINDA_MDBSQL_AST_SCHEMA_ELEMENT_INCLUDED

#include <string>
#include <variant>

#include <fmt/format.h>

#include <mdbsql_ast_table_definition.h>

namespace melinda::mdbsql::ast
{
    using schema_element = std::variant<table_definition>;

    std::string to_string(schema_element const& value);
} // namespace melinda::mdbsql::ast

template<>
struct fmt::formatter<melinda::mdbsql::ast::schema_element>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mdbsql::ast::schema_element const& value,
        FormatContext& ctx) const
    {
        return format_to(ctx.out(), "{}", to_string(value));
    }
};
#endif
