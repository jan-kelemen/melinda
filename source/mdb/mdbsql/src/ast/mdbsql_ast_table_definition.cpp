#include <mdbsql_ast_table_definition.h>

#include <type_traits>

#include <mblcxx_always_false.h>

std::string melinda::mdbsql::ast::to_string(
    melinda::mdbsql::ast::table_definition const& value)
{
    auto const scope_str =
        value.scope ? fmt::format("{} TEMPORARY", value.scope.value()) : "";

    auto const elements_str = std::visit(
        [](auto&& content)
        {
            using namespace std::string_literals;

            using T = std::decay_t<decltype(content)>;
            if constexpr (std::is_same_v<T, table_elements>)
            {
                return to_string(content);
            }
            else
            {
                static_assert(mblcxx::always_false_v<T>,
                    "non-exhaustive visitor!");
                return ""s;
            }
        },
        value.table_contents);

    return fmt::format("CREATE {} TABLE {} ({})",
        scope_str,
        value.name,
        elements_str);
}
