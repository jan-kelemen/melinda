#include <catch2/catch_test_macros.hpp>

#include <source_location>
#include <string>
#include <string_view>

#include <mqlprs_ast_delimited_identifier.h>
#include <mqlprs_ast_identifier.h> // IWYU pragma: keep
#include <mqlprs_ast_regular_identifier.h>
#include <mqlprs_ast_unicode_delimited_identifier.h>

#include <mqlprs_identifier.h> // IWYU pragma: keep
#include <mqlprs_parser.h>

using namespace melinda;

namespace
{
    auto parse = [](std::string_view query)
    { return mqlprs::parse<mqlprs::ast::identifier>(query); };

    template<typename T>
    void check_for_identifier_type(std::string_view expected_body,
        mqlprs::ast::identifier const& id,
        std::source_location const& l = std::source_location::current())
    {
        if (!id.of_type<T>())
        {
            FAIL("<identifier> contains " << id.contained_type()
                                          << " alternative. Line " << l.line());
        }
        REQUIRE(expected_body == id.body());
    }
} // namespace

TEST_CASE("<identifier> recognizes correct variant of identifier")
{
    using namespace std::string_view_literals;

    auto const regular_identifier = "abc"sv;
    check_for_identifier_type<mqlprs::ast::regular_identifier>("abc",
        parse(regular_identifier).value());

    auto const delimited_identifier = R"("abc")"sv;
    check_for_identifier_type<mqlprs::ast::delimited_identifier>("abc",
        parse(delimited_identifier).value());

    auto const unicode_delimited_identifier = R"(U&"abc")"sv;
    check_for_identifier_type<mqlprs::ast::unicode_delimited_identifier>("abc",
        parse(unicode_delimited_identifier).value());
}
