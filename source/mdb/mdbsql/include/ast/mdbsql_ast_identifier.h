#ifndef MELINDA_MDBSQL_AST_IDENTIFIER_INCLUDED
#define MELINDA_MDBSQL_AST_IDENTIFIER_INCLUDED

#include <string>
#include <vector>

#include <fmt/format.h>

namespace melinda::mdbsql::ast
{
    struct [[nodiscard]] identifier final
    {
        std::vector<std::string> parts;

        bool operator==(const identifier&) const = default;
    };

    std::string to_string(const identifier& value);
} // namespace melinda::mdbsql::ast

template<>
struct fmt::formatter<melinda::mdbsql::ast::identifier>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mdbsql::ast::identifier const& value,
        FormatContext& ctx) const
    {
        return format_to(ctx.out(), "{}", to_string(value));
    }
};

#endif
