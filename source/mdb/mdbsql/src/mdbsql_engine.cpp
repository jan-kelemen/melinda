#include <mdbsql_engine.h>

#include <algorithm>

#include <mbltrc_trace.h>

melinda::mdbsql::engine::engine(std::filesystem::path data_directory)
    : data_directory_ {std::move(data_directory)}
{
}

bool melinda::mdbsql::engine::execute(std::string_view)
{
    //    std::optional<statement> parsed;
    //    if (!parse(std::cbegin(query), std::cend(query), parsed))
    //    {
    //        MBLTRC_TRACE_ERROR("Unable to parse query '{}'", query);
    //        return false;
    //    }
    //
    //    statement::create const& create_statement =
    //        get<statement::create>(parsed->value);
    //
    //    if (auto it = std::find(std::cbegin(databases_),
    //            std::cend(databases_),
    //            create_statement.database_name);
    //        it != std::cend(databases_))
    //    {
    //        MBLTRC_TRACE_ERROR("Can't create database '{}', already exists",
    //            create_statement.database_name);
    //        return false;
    //    }
    //
    //    databases_.push_back(create_statement.database_name);
    //    MBLTRC_TRACE_INFO("Created database '{}'",
    //    create_statement.database_name);
    //
    return true;
}
