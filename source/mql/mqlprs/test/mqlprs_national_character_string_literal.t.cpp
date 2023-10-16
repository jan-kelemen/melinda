#include <mqlprs_national_character_string_literal.h> // IWYU pragma: associated

#include <initializer_list>
#include <string>
#include <string_view>

#include <catch2/catch_test_macros.hpp>

#include <mqlast_national_character_string_literal.h>
#include <mqlprs_parser.h>

using namespace melinda;

TEST_CASE("<national character string literal>")
{
    auto parse = [](std::string_view query)
    { return mqlprs::parse<mqlast::national_character_string_literal>(query); };

    struct test_pairs
    {
        std::string_view str;
        std::string_view value;
    };

    auto values = {test_pairs{"N'abc123'", "abc123"},
        test_pairs{"N'abc''123'", "abc'123"},
        test_pairs{"n'abc'   '123'", "abc123"},
        test_pairs{"n'abc' \n  '123'", "abc123"}};

    for (auto&& value : values)
    {
        auto result = parse(value.str);

        if (!result)
        {
            FAIL("Failed for value: " << value.value);
        }
        REQUIRE(result->value() == value.value);
    }
}
