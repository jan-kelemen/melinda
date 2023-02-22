#ifndef MELINDA_MBLTRC_TRACE_LEVEL_INCLUDED
#define MELINDA_MBLTRC_TRACE_LEVEL_INCLUDED

#include <cstdint>
#include <limits>
#include <type_traits>

#include <fmt/core.h>

namespace melinda::mbltrc
{
    enum class [[nodiscard]] trace_level : uint8_t
    {
        debug = 1,
        info = 2,
        warn = 3,
        error = 4,
        fatal = 5,
        always =
            std::numeric_limits<std::underlying_type_t<trace_level>>::max(),
    };
} // namespace melinda::mbltrc

template<>
struct fmt::formatter<melinda::mbltrc::trace_level>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mbltrc::trace_level const& l, FormatContext& ctx)
    {
        char const marker = [&l]() noexcept
        {
            switch (l)
            {
            case melinda::mbltrc::trace_level::debug:
                return 'D';
            case melinda::mbltrc::trace_level::info:
                return 'I';
            case melinda::mbltrc::trace_level::warn:
                return 'W';
            case melinda::mbltrc::trace_level::error:
                return 'E';
            case melinda::mbltrc::trace_level::fatal:
                return 'F';
            case melinda::mbltrc::trace_level::always:
                return 'A';
            default:
                return 'X';
            }
        }();

        return format_to(ctx.out(), "[{}]", marker);
    }
};

#endif
