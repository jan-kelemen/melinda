#include "mdbsql_ast.h"
#include <mdbsql_engine.h>

#include <algorithm>
#include <type_traits>

#include <mblcxx_always_false.h>

#include <mbltrc_trace.h>

#include <mdbsql_parser.h>

namespace
{
    auto as_str(melinda::mdbsql::ast::identifier const& value)
    {
        return std::visit([](auto&& v) { return v.body; }, value);
    }
} // namespace

melinda::mdbsql::engine::engine(std::filesystem::path data_directory)
    : data_directory_{std::move(data_directory)}
{
}

bool melinda::mdbsql::engine::execute(std::string_view query)
{
    auto const sql_schema_statement_visitor = [this](auto&& statement) -> bool
    {
        using T = std::decay_t<decltype(statement)>;
        if constexpr (std::is_same_v<T, ast::schema_definition>)
        {
            if (auto it = std::find(std::cbegin(databases_),
                    std::cend(databases_),
                    as_str(statement.schema_name->back()));
                it != std::cend(databases_))
            {
                MBLTRC_TRACE_ERROR("Can't create schema '{}', already exists",
                    as_str(statement.schema_name->back()));
                return false;
            }

            databases_.push_back(as_str(statement.schema_name->back()));
            MBLTRC_TRACE_INFO("Created schema '{}'",
                as_str(statement.schema_name->back()));
            return true;
        }
        else if constexpr (std::is_same_v<T, ast::table_definition>)
        {
            return false;
        }
        else
        {
            static_assert(mblcxx::always_false_v<T>, "non-exhaustive visitor!");
            return false;
        }
    };

    auto const sql_executable_statement_visitor =
        [&sql_schema_statement_visitor](auto&& statement) -> bool
    {
        using T = std::decay_t<decltype(statement)>;
        if constexpr (std::is_same_v<T, ast::sql_schema_statement>)
        {
            return std::visit(sql_schema_statement_visitor, statement);
        }
        else
        {
            static_assert(mblcxx::always_false_v<T>, "non-exhaustive visitor!");
            return false;
        }
    };

    if (parse_result const statement{parse(query)})
    {
        if (!std::visit(sql_executable_statement_visitor, statement.ok()))
        {
            return false;
        }
        return true;
    }
    return false;
}
