#include <catch2/catch_test_macros.hpp>

#include <mdbsql_engine.h>

#include <mdbsql.t.h>

using namespace melinda;

TEST_CASE("Creating a database succeeds", "[exec]")
{
    mdbsql::engine engine {mdbsql::t::runtime_directory()};
    REQUIRE(engine.execute("CREATE SCHEMA books;"));
}

TEST_CASE("Creating a already existing dabase fails", "[exec]")
{
    mdbsql::engine engine {mdbsql::t::runtime_directory()};
    REQUIRE(engine.execute("CREATE SCHEMA books;"));
    REQUIRE_FALSE(engine.execute("CREATE SCHEMA books;"));
}
