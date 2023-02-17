#include <string>

#include <catch2/catch_test_macros.hpp>

#include <mblcxx_result.h>

using namespace melinda;

TEST_CASE("result constructed by ::ok is considered not to be an error")
{
    mblcxx::result<int> const r = mblcxx::res<int>::ok(1);
    REQUIRE(static_cast<bool>(r));
}

TEST_CASE("result constructed by ::error is considered to be an error")
{
    mblcxx::result<int, char> const r = mblcxx::res<int, char>::error('a');
    REQUIRE(!static_cast<bool>(r));
}

TEST_CASE(
    "result can be constructed by forwarding the values to a T constructor")
{
    struct test_class final
    {
        test_class(int trivial, std::string&& str)
            : trivial_(trivial)
            , str_(std::move(str))
        {
        }

        int trivial_;
        std::string str_;
    };

    mblcxx::result<test_class> const result =
        mblcxx::result<test_class>(std::in_place_type<test_class>, 1, "test");
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

    static_assert(
        std::is_default_constructible_v<mblcxx::result<default_constructible>>);
    static_assert(
        !std::is_copy_constructible_v<mblcxx::result<default_constructible>>);
    static_assert(
        !std::is_move_constructible_v<mblcxx::result<default_constructible>>);
    static_assert(
        !std::is_copy_assignable_v<mblcxx::result<default_constructible>>);
    static_assert(
        !std::is_move_assignable_v<mblcxx::result<default_constructible>>);

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
    static_assert(!std::is_default_constructible_v<mblcxx::result<copyable>>);
    static_assert(std::is_copy_constructible_v<mblcxx::result<copyable>>);
    // static_assert(!std::is_move_constructible_v<mblcxx::result<copyable>>);
    static_assert(std::is_copy_assignable_v<mblcxx::result<copyable>>);

    // static_assert(!std::is_move_assignable_v<mblcxx::result<copyable>>);

    struct moveable final
    {
        moveable() = delete;
        moveable(moveable const&) = delete;
        moveable(moveable&&) noexcept = default;
        moveable& operator=(moveable const&) = delete;
        moveable& operator=(moveable&&) = default;
        ~moveable() noexcept = default;
    };

    static_assert(!std::is_default_constructible_v<mblcxx::result<moveable>>);
    static_assert(!std::is_copy_constructible_v<mblcxx::result<moveable>>);
    static_assert(std::is_move_constructible_v<mblcxx::result<moveable>>);
    static_assert(!std::is_copy_assignable_v<mblcxx::result<moveable>>);
    static_assert(std::is_move_assignable_v<mblcxx::result<moveable>>);
}

TEST_CASE("result::map preserves the error")
{
    mblcxx::result<int, char> const result = mblcxx::res<int, char>::error('a');

    mblcxx::result<std::string, char> const mapped =
        result.map([](int v) { return std::to_string(v); });
    REQUIRE(mapped.error() == 'a');
}

TEST_CASE("result::map preserves the state when single type is used")
{
    mblcxx::result<int, int> value = mblcxx::res<int, int>::ok(5);
    mblcxx::result<int, int> error = mblcxx::res<int, int>::error(3);

    auto callback = [](int) { return 1; };

    REQUIRE(value.map(callback));
    REQUIRE(!error.map(callback));
}

TEST_CASE("result::map_or returns default value in error case")
{
    mblcxx::result<int, char> const result = mblcxx::res<int, char>::error('a');

    long const value = result.map_or([](int v) { return long(v); }, 5);
    REQUIRE(value == 5);
}

TEST_CASE("result::map_or_else invokes correct callback")
{
    mblcxx::result<int, char> value = mblcxx::res<int, char>::ok(5);
    mblcxx::result<int, char> error = mblcxx::res<int, char>::error(3);

    auto value_callback = [](int) { return 3; };
    auto error_callback = [](char) { return 5; };

    REQUIRE(value.map_or_else(value_callback, error_callback) == 3);
    REQUIRE(error.map_or_else(value_callback, error_callback) == 5);
}
