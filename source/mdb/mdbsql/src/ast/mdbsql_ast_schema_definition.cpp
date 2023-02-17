#include <mdbsql_ast_schema_definition.h>

#include <type_traits>

#include <mblcxx_always_false.h>

std::string melinda::mdbsql::ast::to_string(
    melinda::mdbsql::ast::schema_definition const& value)
{
    using namespace std::string_literals;

    auto const ch_or_path_visitor = [](auto&& v)
    {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, identifier>)
        {
            return fmt::format("DEFAULT CHARACTER SET {}", v);
        }
        else if constexpr (std::is_same_v<T, std::vector<identifier>>)
        {
            return fmt::format("PATH {}", fmt::join(v, ", "));
        }
        else
        {
            static_assert(mblcxx::always_false_v<T>, "non-exhaustive visitor!");
            return ""s;
        }
    };

    auto const schema_name_str =
        value.schema_name ? to_string(*value.schema_name) : ""s;
    auto const authorization_str = value.authorization
        ? fmt::format("AUTHORIZATION {}", *value.authorization)
        : ""s;
    auto const first_spec_str = value.character_set_or_path1
        ? std::visit(ch_or_path_visitor, *value.character_set_or_path1)
        : ""s;
    auto const second_spec_str = value.character_set_or_path2
        ? std::visit(ch_or_path_visitor, *value.character_set_or_path2)
        : ""s;
    auto const elements_str = value.elements ? to_string(*value.elements) : ""s;

    return fmt::format("CREATE SCHEMA {} {} {} {} {}",
        schema_name_str,
        authorization_str,
        first_spec_str,
        second_spec_str,
        elements_str);
}
