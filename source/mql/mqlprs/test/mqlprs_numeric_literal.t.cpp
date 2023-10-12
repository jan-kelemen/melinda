#include <mqlprs_numeric_literal.h> // IWYU pragma: associated

#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>

#include <catch2/catch_test_macros.hpp>
#include <tl/expected.hpp>

#include <mqlast_common.h>
#include <mqlast_numeric_literal.h>
#include <mqlprs_parse_error.h>
#include <mqlprs_parser.h>

using namespace melinda;

TEST_CASE("<exact numeric literal>")
{
    auto parse = [](std::string_view v)
    { return mqlprs::parse<mqlast::exact_numeric_literal>(v); };

    struct test_pairs
    {
        std::string_view str;
        std::optional<std::string_view> whole;
        std::optional<std::string_view> decimal;
    };

    auto values = {test_pairs{
                       "2.3",
                       "2",
                       "3",
                   },
        test_pairs{".3", std::nullopt, "3"},
        test_pairs{"2", "2", std::nullopt},
        test_pairs{"2.", "2", std::nullopt}};

    for (auto&& value : values)
    {
        auto result = parse(value.str);
        REQUIRE(result->whole() == value.whole);
        REQUIRE(result->decimal() == value.decimal);
    }
}

TEST_CASE("<approximate numeric literal>")
{
    auto parse = [](std::string_view v)
    { return mqlprs::parse<mqlast::approximate_numeric_literal>(v); };

    struct test_pairs
    {
        std::string_view str;
        mqlast::exact_numeric_literal mantissa;
        std::optional<mqlast::sign> sign;
        std::string_view exponent;
    };

    auto values = {
        test_pairs{"2.3E4", {"2", "3"}, std::nullopt, "4"},
        test_pairs{".3E4", {std::nullopt, "3"}, std::nullopt, "4"},
        test_pairs{"2E4", {"2"}, std::nullopt, "4"},
        test_pairs{"2.E4", {"2"}, std::nullopt, "4"},
        test_pairs{"2.3e4", {"2", "3"}, std::nullopt, "4"},
        test_pairs{"2.3E+4", {"2", "3"}, mqlast::sign::plus, "4"},
        test_pairs{"2.3E-4", {"2", "3"}, mqlast::sign::minus, "4"},
        test_pairs{"2.3E45", {"2", "3"}, std::nullopt, "45"},
    };

    for (auto&& value : values)
    {
        auto result = parse(value.str);
        REQUIRE(result->mantissa() == value.mantissa);
        REQUIRE(result->sign() == value.sign);
        REQUIRE(result->exponent() == value.exponent);
    }
}

TEST_CASE("<unsigned numeric literal>")
{
    auto parse = [](std::string_view v)
    { return mqlprs::parse<mqlast::unsigned_numeric_literal>(v); };

    struct test_pairs
    {
        std::string_view str;
        mqlast::unsigned_numeric_literal expected;
    };

    SECTION("Parses to <approximate numeric literal>")
    {
        auto values = {
            test_pairs{"2.3E4",
                mqlast::approximate_numeric_literal{
                    mqlast::exact_numeric_literal{"2", "3"},
                    std::nullopt,
                    "4"}},
            test_pairs{".3E45",
                mqlast::approximate_numeric_literal{
                    mqlast::exact_numeric_literal{std::nullopt, "3"},
                    std::nullopt,
                    "45"}},
            test_pairs{"2.E45",
                mqlast::approximate_numeric_literal{
                    mqlast::exact_numeric_literal{"2"},
                    std::nullopt,
                    "45"}},
            test_pairs{"2E45",
                mqlast::approximate_numeric_literal{
                    mqlast::exact_numeric_literal{"2"},
                    std::nullopt,
                    "45"}},
        };

        for (auto&& value : values)
        {
            auto result = parse(value.str);
            if (!result)
            {
                FAIL("Failed for value: " << value.str);
            }
            REQUIRE(result == value.expected);
        }
    }

    SECTION("Parses to <exact numeric literal>")
    {
        auto values = {
            test_pairs{"2.3", mqlast::exact_numeric_literal{"2", "3"}},
            test_pairs{".345",
                mqlast::exact_numeric_literal{std::nullopt, "345"}},
            test_pairs{"2.", mqlast::exact_numeric_literal{"2", std::nullopt}},
            test_pairs{"2", mqlast::exact_numeric_literal{"2", std::nullopt}},
        };

        for (auto&& value : values)
        {
            auto result = parse(value.str);
            REQUIRE(result == value.expected);
        }
    }
}

TEST_CASE("<signed numeric literal>")
{
    auto parse = [](std::string_view v)
    { return mqlprs::parse<mqlast::signed_numeric_literal>(v); };

    struct test_pairs
    {
        std::string_view str;
        mqlast::signed_numeric_literal expected;
    };

    SECTION("Parses to <approximate numeric literal>")
    {
        auto values = {test_pairs{"+2.3E4",
                           mqlast::signed_numeric_literal{mqlast::sign::plus,
                               mqlast::approximate_numeric_literal{
                                   mqlast::exact_numeric_literal{"2", "3"},
                                   std::nullopt,
                                   "4"}}},
            test_pairs{"-.3E45",
                mqlast::signed_numeric_literal{mqlast::sign::minus,
                    mqlast::approximate_numeric_literal{
                        mqlast::exact_numeric_literal{std::nullopt, "3"},
                        std::nullopt,
                        "45"}}},
            test_pairs{"2.E45",
                mqlast::signed_numeric_literal{
                    mqlast::approximate_numeric_literal{
                        mqlast::exact_numeric_literal{"2"},
                        std::nullopt,
                        "45"}}}};

        for (auto&& value : values)
        {
            auto result = parse(value.str);
            REQUIRE(result == value.expected);
        }
    }

    SECTION("Parses to <exact numeric literal>")
    {
        auto values = {
            test_pairs{"+2.3",
                mqlast::signed_numeric_literal{mqlast::sign::plus,
                    mqlast::exact_numeric_literal{"2", "3"}}},
            test_pairs{"-.3",
                mqlast::signed_numeric_literal{mqlast::sign::minus,
                    mqlast::exact_numeric_literal{std::nullopt, "3"}}},
            test_pairs{"2.",
                mqlast::signed_numeric_literal{
                    mqlast::exact_numeric_literal{"2"}}},
        };

        for (auto&& value : values)
        {
            auto result = parse(value.str);
            REQUIRE(result == value.expected);
        }
    }
}
