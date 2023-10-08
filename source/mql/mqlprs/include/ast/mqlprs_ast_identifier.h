#ifndef MELINDA_MQLPRS_AST_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_AST_IDENTIFIER_INCLUDED

#include <variant>
#include <vector>

#include <mqlprs_ast_delimited_identifier.h>
#include <mqlprs_ast_regular_identifier.h>
#include <mqlprs_ast_unicode_delimited_identifier.h>

namespace melinda::mqlprs::ast
{
    using identifier = std::variant<regular_identifier,
        delimited_identifier,
        unicode_delimited_identifier>;

    using multipart_identifier = std::vector<identifier>;
} // namespace melinda::mqlprs::ast

#endif
