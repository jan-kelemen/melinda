#ifndef MELINDA_MQLAST_SYNTAX_TREE_INCLUDED
#define MELINDA_MQLAST_SYNTAX_TREE_INCLUDED

#include <string>
#include <variant>
#include <vector>

namespace melinda::mqlast
{
    struct create_schema_command final
    {
        std::string schema_name;
    };

    using command = std::variant<create_schema_command>;

    struct syntax_tree final
    {
        std::vector<command> commands;
    };
} // namespace melinda::mqlast

#endif
