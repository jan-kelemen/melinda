#ifndef MELINDA_MBLTRC_TRACE_INCLUDED
#define MELINDA_MBLTRC_TRACE_INCLUDED

#include <concepts>
#include <memory>
#include <source_location>
#include <string_view>
#include <thread>
#include <type_traits>

#include <fmt/format.h>

#include <mbltrc_sink.h>
#include <mbltrc_timestamp.h>
#include <mbltrc_trace_level.h>

namespace melinda::mbltrc::detail
{
    template<typename T>
    concept sink_ptr = std::convertible_to<T, sink const*> ||
        std::convertible_to<decltype(*std::declval<T>()), sink&>;

    template<typename... T>
    [[nodiscard]] constexpr std::string format_message(std::string_view format,
        T&&... args)
    {
        return fmt::format(fmt::runtime(format), std::forward<T>(args)...);
    }

    [[nodiscard]] constexpr bool should_trace_message(sink const& sink,
        trace_level const level) noexcept
    {
        return sink.should_trace_message(level);
    }

    template<sink_ptr T>
    [[nodiscard]] constexpr bool should_trace_message(T sink,
        trace_level const level) noexcept
    {
        return sink && should_trace_message(*sink, level);
    }

    constexpr void trace(sink const& sink,
        message_origin const& origin,
        timestamp const& time,
        trace_level level,
        std::string_view message) noexcept
    {
        sink.trace(origin, time, level, message);
    }

    template<sink_ptr T>
    constexpr void trace(T sink,
        message_origin const& origin,
        timestamp const& time,
        trace_level level,
        std::string_view message) noexcept
    {
        trace(*sink, origin, time, level, message);
    }

    std::shared_ptr<sink> process_sink();
} // namespace melinda::mbltrc::detail

namespace melinda::mbltrc
{
    void register_process_sink(std::weak_ptr<sink> sink);
} // namespace melinda::mbltrc

#define MBLTRC_TRACE(f, l, level, sink, ...) \
    do \
    { \
        melinda::mbltrc::timestamp const \
            now_b015cf7e_932f_4d99_822a_b863763c6e23{ \
                melinda::mbltrc::current_timestamp()}; \
        melinda::mbltrc::message_origin const \
            origin_b015cf7e_932f_4d99_822a_b863763c6e23{ \
                .source = std::source_location::current(), \
                .thread_id = std::this_thread::get_id()}; \
        if (melinda::mbltrc::detail::should_trace_message(sink, level)) \
        { \
            melinda::mbltrc::detail::trace(sink, \
                origin_b015cf7e_932f_4d99_822a_b863763c6e23, \
                now_b015cf7e_932f_4d99_822a_b863763c6e23, \
                level, \
                melinda::mbltrc::detail::format_message(__VA_ARGS__)); \
        } \
    } while (false)

#define MBLTRC_TRACE_SINK_DEBUG(sink, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::debug, \
        sink, \
        __VA_ARGS__)

#define MBLTRC_TRACE_SINK_INFO(sink, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::info, \
        sink, \
        __VA_ARGS__)

#define MBLTRC_TRACE_SINK_WARN(sink, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::warn, \
        sink, \
        __VA_ARGS__)

#define MBLTRC_TRACE_SINK_ERROR(sink, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::error, \
        sink, \
        __VA_ARGS__)

#define MBLTRC_TRACE_SINK_FATAL(sink, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::fatal, \
        sink, \
        __VA_ARGS__)

#define MBLTRC_TRACE_SINK_ALWAYS(sink, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::always, \
        sink, \
        __VA_ARGS__)

#define MBLTRC_TRACE_DEBUG(...) \
    MBLTRC_TRACE_SINK_DEBUG(melinda::mbltrc::detail::process_sink(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_INFO(...) \
    MBLTRC_TRACE_SINK_INFO(melinda::mbltrc::detail::process_sink(), __VA_ARGS__)

#define MBLTRC_TRACE_WARN(...) \
    MBLTRC_TRACE_SINK_WARN(melinda::mbltrc::detail::process_sink(), __VA_ARGS__)

#define MBLTRC_TRACE_ERROR(...) \
    MBLTRC_TRACE_SINK_ERROR(melinda::mbltrc::detail::process_sink(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_FATAL(...) \
    MBLTRC_TRACE_SINK_FATAL(melinda::mbltrc::detail::process_sink(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_ALWAYS(...) \
    MBLTRC_TRACE_SINK_ALWAYS(melinda::mbltrc::detail::process_sink(), \
        __VA_ARGS__)

#endif
