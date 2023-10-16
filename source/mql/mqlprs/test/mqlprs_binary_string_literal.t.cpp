#include <mqlprs_binary_string_literal.h>

#include <catch2/catch_test_macros.hpp>

#include <mqlprs_parser.h>
#include <vector>

using namespace melinda;

namespace
{
    template<typename... Args>
    std::vector<std::byte> from_ints(Args... args)
    {
        std::vector<std::byte> rv;

        (rv.emplace_back(static_cast<std::byte>(args)), ...);

        return rv;
    }
} // namespace

TEST_CASE("<binary string literal>")
{
    auto parse = [](std::string_view query)
    { return mqlprs::parse<mqlast::binary_string_literal>(query); };

    struct test_pairs
    {
        std::string_view str;
        std::vector<std::byte> value;
    };

    auto values = {
        test_pairs{"x'0123456789abcdef'",
            from_ints(0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF)},
        test_pairs{"X'0123456789ABCDEF'",
            from_ints(0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF)},
        test_pairs{"X'01234567''89ABCDEF'",
            from_ints(0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF)},
        test_pairs{"X'01234567'     '89ABCDEF'",
            from_ints(0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF)},
        test_pairs{"x' 0 12  345  67'     '89ABCDEF'",
            from_ints(0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF)}};

    for (auto&& value : values)
    {
        auto result = parse(value.str);

        if (!result)
        {
            FAIL("Failed for value: " << value.str);
        }
        REQUIRE(result->value() == value.value);
    }
}
