#ifndef MELINDA_MBLTRC_TIMESTAMP_INCLUDED
#define MELINDA_MBLTRC_TIMESTAMP_INCLUDED

#include <cstdint>

#include <fmt/core.h>

namespace melinda::mbltrc
{
    struct timestamp
    {
        int16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hours;
        uint8_t minutes;
        uint8_t seconds;
        uint16_t milliseconds;
    };

    [[nodiscard]] timestamp current_timestamp() noexcept;
} // namespace melinda::mbltrc

template<>
struct fmt::formatter<melinda::mbltrc::timestamp>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mbltrc::timestamp const& t, FormatContext& ctx)
    {
        return format_to(ctx.out(),
            "{:04}-{:02}-{:02}T{:02}:{:02}:{:02}:{:03}",
            t.year,
            t.month,
            t.day,
            t.hours,
            t.minutes,
            t.seconds,
            t.milliseconds);
    }
};

#endif
