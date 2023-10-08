#ifndef MELINDA_MDBSQL_AST_IDENTIFIER_INCLUDED
#define MELINDA_MDBSQL_AST_IDENTIFIER_INCLUDED

#include <variant>
#include <vector>

#include <mdbsql_ast_delimited_identifier.h>
#include <mdbsql_ast_regular_identifier.h>
#include <mdbsql_ast_unicode_delimited_identifier.h>

namespace melinda::mdbsql::ast
{
    using identifier = std::variant<regular_identifier,
        delimited_identifier,
        unicode_delimited_identifier>;

    using multipart_identifier = std::vector<identifier>;
} // namespace melinda::mdbsql::ast

#endif
