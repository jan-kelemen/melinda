#include <string>

#include <catch2/catch.hpp>

#include "result.h"

TEST_CASE("result constructed by ::ok is considered not to be an error")
{
    mel::cppex::result<int> const r = mel::cppex::res::ok<int>(1);
    REQUIRE(static_cast<bool>(r));
}

TEST_CASE("result constructed by ::error is considered to be an error")
{
    mel::cppex::result<int, char> const r =
        mel::cppex::res::error<int, char>('a');
    REQUIRE(!static_cast<bool>(r));
}

TEST_CASE(
    "result can be constructed by forwarding the values to a T constructor")
{
    struct test_class final
    {
        test_class(int trivial, std::string&& str)
            : trivial_(trivial)
            , str_(std::move(str)) {};

        int trivial_;
        std::string str_;
    };

    mel::cppex::result<test_class> const result =
        mel::cppex::result<test_class>(std::in_place_type<test_class>,
            1,
            "test");
    REQUIRE(static_cast<bool>(result));

    test_class const& value = result.ok();
    REQUIRE(value.trivial_ == 1);
    REQUIRE(value.str_ == "test");
}

TEST_CASE("Special member functions are propagated from containing type")
{
    struct default_constructible final
    {
        default_constructible() = default;
        default_constructible(default_constructible const&) = delete;
        default_constructible(default_constructible&&) noexcept = delete;
        default_constructible& operator=(default_constructible const&) = delete;
        default_constructible& operator=(
            default_constructible&&) noexcept = delete;
        ~default_constructible() noexcept = default;
    };

    static_assert(std::is_default_constructible_v<
        mel::cppex::result<default_constructible>>);
    static_assert(!std::is_copy_constructible_v<
                  mel::cppex::result<default_constructible>>);
    static_assert(!std::is_move_constructible_v<
                  mel::cppex::result<default_constructible>>);
    static_assert(
        !std::is_copy_assignable_v<mel::cppex::result<default_constructible>>);
    static_assert(
        !std::is_move_assignable_v<mel::cppex::result<default_constructible>>);

    struct copyable final
    {
        copyable() = delete;
        copyable(copyable const&) = default;
        copyable(copyable&&) noexcept = delete;
        copyable& operator=(copyable const&) = default;
        copyable& operator=(copyable&&) noexcept = delete;
        ~copyable() noexcept = default;
    };

    // std::is_move_constructible/assignable bind to a const& overload
    // if the type isn't movable, so it can't be checked?
    static_assert(
        !std::is_default_constructible_v<mel::cppex::result<copyable>>);
    static_assert(std::is_copy_constructible_v<mel::cppex::result<copyable>>);
    // static_assert(!std::is_move_constructible_v<mel::cppex::result<copyable>>);
    static_assert(std::is_copy_assignable_v<mel::cppex::result<copyable>>);
    // static_assert(!std::is_move_assignable_v<mel::cppex::result<copyable>>);

    struct moveable final
    {
        moveable() = delete;
        moveable(moveable const&) = delete;
        moveable(moveable&&) noexcept = default;
        moveable& operator=(moveable const&) = delete;
        moveable& operator=(moveable&&) = default;
        ~moveable() noexcept = default;
    };

    static_assert(
        !std::is_default_constructible_v<mel::cppex::result<moveable>>);
    static_assert(!std::is_copy_constructible_v<mel::cppex::result<moveable>>);
    static_assert(std::is_move_constructible_v<mel::cppex::result<moveable>>);
    static_assert(!std::is_copy_assignable_v<mel::cppex::result<moveable>>);
    static_assert(std::is_move_assignable_v<mel::cppex::result<moveable>>);
}

TEST_CASE("result::map preserves the error")
{
    mel::cppex::result<int, char> const result =
        mel::cppex::res::error<int, char>('a');

    mel::cppex::result<std::string, char> const mapped =
        result.map([](int v) { return std::to_string(v); });
    REQUIRE(mapped.error() == 'a');
}

TEST_CASE("result::map preserves the state when single type is used")
{
    mel::cppex::result<int, int> value = mel::cppex::res::ok<int, int>(5);
    mel::cppex::result<int, int> error = mel::cppex::res::error<int, int>(3);

    auto callback = [](int) { return 1; };

    REQUIRE(value.map(callback));
    REQUIRE(!error.map(callback));
}

TEST_CASE("result::map_or returns default value in error case")
{
    mel::cppex::result<int, char> const result =
        mel::cppex::res::error<int, char>('a');

    long const value = result.map_or([](int v) { return long(v); }, 5);
    REQUIRE(value == 5);
}

TEST_CASE("result::map_or_else invokes correct callback")
{
    mel::cppex::result<int, char> value = mel::cppex::res::ok<int, char>(5);
    mel::cppex::result<int, char> error = mel::cppex::res::error<int, char>(3);

    auto value_callback = [](int) { return 3; };
    auto error_callback = [](char) { return 5; };

    REQUIRE(value.map_or_else(value_callback, error_callback) == 3);
    REQUIRE(error.map_or_else(value_callback, error_callback) == 5);
}
