#include <mqlprs_unicode_character_string_literal.h> // IWYU pragma: associated

#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <mqlast_identifier.h>
#include <mqlast_regular_identifier.h>
#include <mqlast_unicode_character_string_literal.h>
#include <mqlprs_parser.h>

using namespace melinda;

namespace
{
    template<typename... Args>
    mqlast::multipart_identifier from_strings(Args... args)
    {
        std::vector<mqlast::identifier> rv;

        (rv.emplace_back(mqlast::regular_identifier{args}), ...);

        return {std::move(rv)};
    }
} // namespace

TEST_CASE("<unicode character string literal>")
{
    auto parse = [](std::string_view query)
    { return mqlprs::parse<mqlast::unicode_character_string_literal>(query); };

    struct test_pairs
    {
        std::string_view str;
        std::optional<mqlast::multipart_identifier> character_set;
        std::string_view value;
        char escape_character = '\\';
    };

    auto values = {test_pairs{"U'abc123'", std::nullopt, "abc123"},
        test_pairs{"U'abc''123'", std::nullopt, "abc'123"},
        test_pairs{"U'abc'   '123'", std::nullopt, "abc123"},
        test_pairs{"U'abc' \n  '123'", std::nullopt, "abc123"},
        test_pairs{"_ABC u'abc123'", from_strings("ABC"), "abc123"},
        test_pairs{"_ABC.DEF u'abc123'", from_strings("ABC", "DEF"), "abc123"},
        test_pairs{"_ABC.DEF u'abyycy00611y+00006123' UESCAPE 'y'",
            from_strings("ABC", "DEF"),
            "abyca1a23",
            'y'},
        test_pairs{"_ABC.DEF u'abc123' UESCAPE 'y'",
            from_strings("ABC", "DEF"),
            "abc123",
            'y'}};

    for (auto&& value : values)
    {
        auto result = parse(value.str);

        if (!result)
        {
            FAIL("Failed for value: " << value.str);
        }
        REQUIRE(result->character_set() == value.character_set);
        REQUIRE(result->value() == value.value);
        REQUIRE(result->escape_character() == value.escape_character);
    }
}
