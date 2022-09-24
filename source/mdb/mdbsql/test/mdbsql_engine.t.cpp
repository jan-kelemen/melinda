#include <catch2/catch_test_macros.hpp>

#include <mdbsql_engine.h>

using namespace melinda;

TEST_CASE("Creating a database succeeds", "[exec]")
{
    mdbsql::engine engine;
    REQUIRE(engine.execute("CREATE DATABASE books;"));
}

TEST_CASE("Creating a already existing dabase fails", "[exec]")
{
    mdbsql::engine engine;
    REQUIRE(engine.execute("CREATE DATABASE books;"));
    REQUIRE_FALSE(engine.execute("CREATE DATABASE books;"));
}
