#ifndef MELINDA_MDBSQL_PARSER_INCLUDED
#define MELINDA_MDBSQL_PARSER_INCLUDED

#include <string_view>

#include <mblcxx_result.h>

#include <mdbsql_ast_sql_executable_statement.h>
#include <mdbsql_parser_parse_error.h>

namespace melinda::mdbsql
{
    using parse_result =
        mblcxx::result<ast::sql_executable_statement, parser::parse_error>;

    parse_result parse(std::string_view statement);
} // namespace melinda::mdbsql
#endif
