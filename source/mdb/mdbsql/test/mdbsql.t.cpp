#include <mdbsql.t.h>

#include <catch2/catch_test_macros.hpp>

#include <stdlib.h>

using namespace melinda;

std::filesystem::path mdbsql::t::runtime_directory()
{
#ifndef WIN32
    char* const env = secure_getenv("MELINDA_TEST_DIRECTORY");
#else
    char* const env = getenv("MELINDA_TEST_DIRECTORY");
#endif

    REQUIRE(env);
    return {env};
}
