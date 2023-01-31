#ifndef MELINDA_MDBSQL_AST_INCLUDED
#define MELINDA_MDBSQL_AST_INCLUDED

#include <string>

namespace melinda::mdbsql::ast
{
    struct sql_executable_statement
    {
        std::string identifier;
    };

} // namespace melinda::mdbsql::ast

#endif
