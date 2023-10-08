#ifndef MELINDA_MQLPRS_AST_DELIMITED_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_AST_DELIMITED_IDENTIFIER_INCLUDED

#include <string>

namespace melinda::mqlprs::ast
{
    struct [[nodiscard]] delimited_identifier final
    {
        std::string body;
    };

} // namespace melinda::mqlprs::ast

#endif
