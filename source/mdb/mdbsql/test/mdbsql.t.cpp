#include <mdbsql.t.h>

#include <catch2/catch_test_macros.hpp>

#include <stdlib.h>

using namespace melinda;

std::filesystem::path mdbsql::t::runtime_directory()
{
    char* const env = secure_getenv("MELINDA_TEST_DIRECTORY");
    REQUIRE(env);
    return {env};
}
