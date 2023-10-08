#include <mdbsql_engine.h>

#include <utility>

#include <mbltrc_trace.h>

melinda::mdbsql::engine::engine(std::filesystem::path data_directory)
    : data_directory_{std::move(data_directory)}
{
}

bool melinda::mdbsql::engine::execute(std::string_view query)
{
    MBLTRC_TRACE_ERROR("Execution failed for query '{}'", query);
    return false;
}
