#ifndef MELINDA_MQLPRS_AST_REGULAR_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_AST_REGULAR_IDENTIFIER_INCLUDED

#include <string>

namespace melinda::mqlprs::ast
{
    struct [[nodiscard]] regular_identifier final
    {
        std::string body;
    };

} // namespace melinda::mqlprs::ast

#endif
