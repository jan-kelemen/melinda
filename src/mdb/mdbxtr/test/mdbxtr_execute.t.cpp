#include <mdbxtr_execute.hpp>

#include <mblcxx_expected.hpp>

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <array>
#include <filesystem>
#include <random>

using namespace melinda;

namespace
{
    struct working_directory_guard final
    {
    public:
        working_directory_guard()
        {
            static std::mt19937 engine{std::random_device()()};
            std::uniform_int_distribution<int> dist('a', 'z');

            std::array<char, 20> buffer{};
            std::generate_n(buffer.begin(),
                buffer.size() - 1,
                [&dist]() mutable { return static_cast<char>(dist(engine)); });

            current_ = previous_ / buffer.data();

            std::filesystem::create_directory(current_);
            std::filesystem::current_path(current_);
        }

        working_directory_guard(working_directory_guard const&) = delete;
        working_directory_guard(working_directory_guard&&) noexcept = delete;

    public:
        ~working_directory_guard()
        {
            std::filesystem::current_path(previous_);
            std::filesystem::remove_all(current_);
        }

    public:
        working_directory_guard& operator=(
            working_directory_guard const&) = delete;
        working_directory_guard& operator=(
            working_directory_guard&&) noexcept = delete;

    private:
        std::filesystem::path previous_{std::filesystem::current_path()};
        std::filesystem::path current_{};
    };
} // namespace

TEST_CASE("mdbxtr::execute empty_statement")
{
    REQUIRE(mdbxtr::execute("") == 0);
    ;
}

TEST_CASE("mdbxtr::execute create schema")
{
    SECTION("single statement")
    {
        working_directory_guard const guard;
        REQUIRE(mdbxtr::execute("create schema a") == 1);
    }

    SECTION("multiple statements")
    {
        working_directory_guard const guard;
        REQUIRE(mdbxtr::execute("create schema a; create schema b") == 2);
    }
}
