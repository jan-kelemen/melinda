#include <mqlprs_datetime_literal.h> // IWYU pragma: associated

#include <initializer_list>
#include <optional>
#include <source_location>
#include <string_view>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>
#include <tl/expected.hpp>

#include <mqlast_common.h>
#include <mqlprs_parse_error.h>
#include <mqlprs_parser.h>

using namespace melinda;

namespace
{
    template<typename T>
    void check_for_literal_type(T const& value,
        mqlast::datetime_literal const& literal,
        std::source_location const& l = std::source_location::current())
    {
        if (!literal.of_type<std::decay_t<T>>())
        {
            FAIL("<datetime literal> contains " << literal.contained_type()
                                                << " alternative. Line "
                                                << l.line());
        }
        REQUIRE(mqlast::datetime_literal{value} == literal);
    }
} // namespace

TEST_CASE("<datetime literal>")
{
    using namespace std::string_view_literals;

    SECTION("<date literal>")
    {
        auto parse = [](auto&& v) {
            return mqlprs::parse<mqlast::date_literal>(
                std::forward<decltype(v)>(v));
        };

        auto dates = {"DATE '19-10-2023'"sv,
            "daTe '19-10-2023'"sv,
            "date'19-10-2023'"sv};
        for (auto&& value : dates)
        {
            auto result{parse(value)};
            REQUIRE(result->day() == 19);
            REQUIRE(result->month() == 10);
            REQUIRE(result->year() == 2023);
        }
    }

    SECTION("<time literal>")
    {
        auto parse = [](auto&& v) {
            return mqlprs::parse<mqlast::time_literal>(
                std::forward<decltype(v)>(v));
        };

        SECTION("<time literal> without time zone")
        {
            auto times = {"TIME '19:20:33'"sv,
                "TimE '19:20:33'"sv,
                "time'19:20:33'"sv};
            for (auto&& value : times)
            {
                auto result{parse(value)};
                REQUIRE(result->hours() == 19);
                REQUIRE(result->minutes() == 20);
                REQUIRE(result->seconds() == 33);
            }
        }

        SECTION("<time literal> with time zone")
        {
            auto times = {"TIME '19:20:33+10:11'"sv,
                "TimE '19:20:33+10:11'"sv,
                "time'19:20:33+10:11'"sv};
            for (auto&& value : times)
            {
                auto result{parse(value)};
                REQUIRE(result->hours() == 19);
                REQUIRE(result->minutes() == 20);
                REQUIRE(result->seconds() == 33);
                REQUIRE(result->time_zone() ==
                    mqlast::time_zone_literal{mqlast::sign::plus,
                        uint8_t{10},
                        uint8_t{11}});
            }

            auto minus_time_zone_result{parse("TIME '19:20:33-10:11'")};
            REQUIRE(minus_time_zone_result->time_zone() ==
                mqlast::time_zone_literal{mqlast::sign::minus,
                    uint8_t{10},
                    uint8_t{11}});
        }

        SECTION("<time literal> with fraction")
        {
            auto fraction_time_result{parse("TIME'16:45:23.911'")};
            REQUIRE(fraction_time_result->fraction() == 911);
        }

        SECTION("<time literal> with all optional parts")
        {
            auto result{parse("time'16:45:23.911-10:11'")};
            REQUIRE(result ==
                mqlast::time_literal{uint8_t{16},
                    uint8_t{45},
                    uint8_t{23},
                    uint32_t{911},
                    mqlast::time_zone_literal{mqlast::sign::minus,
                        uint8_t{10},
                        uint8_t{11}}});
        }
    }

    SECTION("<timestamp literal>")
    {
        auto parse = [](auto&& v)
        {
            return mqlprs::parse<mqlast::timestamp_literal>(
                std::forward<decltype(v)>(v));
        };

        auto timestamps = {"TIMESTAMP '19-10-2023 16:45:23.911-10:11'"sv,
            "TiMEsTAmP '19-10-2023 16:45:23.911-10:11'"sv,
            "timestamp'19-10-2023 16:45:23.911-10:11'"sv};
        for (auto&& value : timestamps)
        {
            auto result{parse(value)};
            REQUIRE(result->date() ==
                mqlast::date_literal{uint8_t{19}, uint8_t{10}, uint16_t{2023}});
            REQUIRE(result->time() ==
                mqlast::time_literal{uint8_t{16},
                    uint8_t{45},
                    uint8_t{23},
                    uint32_t{911},
                    mqlast::time_zone_literal{mqlast::sign::minus,
                        uint8_t{10},
                        uint8_t{11}}});
        }
    }

    auto parse = [](auto&& v)
    {
        return mqlprs::parse<mqlast::datetime_literal>(
            std::forward<decltype(v)>(v));
    };

    mqlast::date_literal date{uint8_t{19}, uint8_t{10}, uint16_t{2023}};
    mqlast::time_literal time_{uint8_t{16},
        uint8_t{45},
        uint8_t{23},
        uint32_t{911},
        mqlast::time_zone_literal{mqlast::sign::minus,
            uint8_t{10},
            uint8_t{11}}};

    mqlast::timestamp_literal timestamp{date, time_};

    check_for_literal_type(timestamp,
        *parse("TIMESTAMP '19-10-2023 16:45:23.911-10:11'"));
    check_for_literal_type(date, *parse("DATE '19-10-2023'"));
    check_for_literal_type(time_, *parse("TIME '16:45:23.911-10:11'"));
}
