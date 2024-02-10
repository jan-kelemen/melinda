#include <catch2/catch_test_macros.hpp>

#include <vector>

TEST_CASE("vectors can be sized and resized", "[vector]")
{
    // This setup will be done 4 times in total, once for each section
    std::vector<int> vec(5);

    REQUIRE(vec.size() == 5);// cppcheck-suppress knownConditionTrueFalse
    REQUIRE(vec.capacity() >= 5);

    SECTION("resizing bigger changes size and capacity")
    {
        vec.resize(10);

        REQUIRE(vec.size() == 10); // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(vec.capacity() >= 10);
    }
    SECTION("resizing smaller changes size but not capacity")
    {
        vec.resize(0);

        //NOLINTNEXTLINE
        REQUIRE(vec.size() == 0); // cppcheck-suppress knownConditionTrueFalse
        REQUIRE(vec.capacity() >= 5);
    }
    SECTION("reserving bigger changes capacity but not size")
    {
        vec.reserve(10);

        REQUIRE(vec.size() == 5);
        REQUIRE(vec.capacity() >= 10);
    }
    SECTION("reserving smaller does not change size or capacity")
    {
        vec.reserve(0);

        REQUIRE(vec.size() == 5);
        REQUIRE(vec.capacity() >= 5);
    }
}
