#ifndef MELINDA_MQLPRS_AST_UNICODE_DELIMITED_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_AST_UNICODE_DELIMITED_IDENTIFIER_INCLUDED

#include <string>

namespace melinda::mqlprs::ast
{
    struct [[nodiscard]] unicode_delimited_identifier final
    {
        std::string body;
        char escape_character;
    };

} // namespace melinda::mqlprs::ast

#endif
