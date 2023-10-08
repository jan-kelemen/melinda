#ifndef MELINDA_MDBSQL_AST_UNICODE_DELIMITED_IDENTIFIER_INCLUDED
#define MELINDA_MDBSQL_AST_UNICODE_DELIMITED_IDENTIFIER_INCLUDED

#include <string>

namespace melinda::mdbsql::ast
{
    struct [[nodiscard]] unicode_delimited_identifier final
    {
        std::string body;
        char escape_character;
    };

} // namespace melinda::mdbsql::ast

#endif
