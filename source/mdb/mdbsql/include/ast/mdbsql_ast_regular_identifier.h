#ifndef MELINDA_MDBSQL_AST_REGULAR_IDENTIFIER_INCLUDED
#define MELINDA_MDBSQL_AST_REGULAR_IDENTIFIER_INCLUDED

#include <string>
#include <vector>

namespace melinda::mdbsql::ast
{
    struct [[nodiscard]] regular_identifier final
    {
        std::string body;
        bool delimited;
    };

} // namespace melinda::mdbsql::ast

#endif
