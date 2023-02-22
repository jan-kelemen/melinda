#ifndef MELINDA_MBLTRC_TRACE_INCLUDED
#define MELINDA_MBLTRC_TRACE_INCLUDED

#include <cstdint>
#include <filesystem>
#include <limits>
#include <memory>
#include <string_view>
#include <type_traits>

#include <fmt/format.h>

#include <mbltrc_timestamp.h>
#include <mbltrc_trace_handle.h>
#include <mbltrc_trace_level.h>
#include <mbltrc_trace_options.h>

namespace melinda::mbltrc::detail
{
    constexpr std::string_view file_name_only(std::string_view path)
    {
        if (auto last_slash = path.find_last_of('/');
            last_slash != std::string_view::npos)
        {
            return path.substr(last_slash + 1);
        }
        return path;
    }

    template<typename... T>
    [[nodiscard]] std::string format_message(std::string_view filename,
        uintmax_t const line,
        melinda::mbltrc::trace_level const level,
        std::string_view format,
        T&&... args)
    {
        // TODO-JK: Can this be done without extra reformatting?
        timestamp const timestamp = current_timestamp();
        std::string const real_format = fmt::format("{} {} {} [{}:{}]\n",
            timestamp,
            level,
            format,
            filename,
            line);
        return fmt::format(fmt::runtime(real_format), std::forward<T>(args)...);
    }

    [[nodiscard]] inline bool should_trace_message(trace_handle const& handle,
        trace_level const level) noexcept
    {
        return handle.should_trace_message(level);
    }

    [[nodiscard]] inline bool should_trace_message(trace_handle const* handle,
        trace_level const level) noexcept
    {
        return handle && should_trace_message(*handle, level);
    }

    inline void trace(trace_handle const& handle,
        std::string_view message) noexcept
    {
        handle.trace(message);
    }

    inline void trace(trace_handle const* handle,
        std::string_view message) noexcept
    {
        trace(*handle, message);
    }
} // namespace melinda::mbltrc::detail

#define MBLTRC_TRACE(f, l, level, handle, ...) \
    do \
    { \
        std::string_view const filename_b015cf7e_932f_4d99_822a_b863763c6e23 = \
            melinda::mbltrc::detail::file_name_only(f); \
        if (melinda::mbltrc::detail::should_trace_message(handle, level)) \
        { \
            melinda::mbltrc::detail::trace(handle, \
                melinda::mbltrc::detail::format_message( \
                    filename_b015cf7e_932f_4d99_822a_b863763c6e23, \
                    l, \
                    level, \
                    __VA_ARGS__)); \
        } \
    } while (false)

#define MBLTRC_TRACE_HANDLE_DEBUG(handle, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::debug, \
        handle, \
        __VA_ARGS__)

#define MBLTRC_TRACE_HANDLE_INFO(handle, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::info, \
        handle, \
        __VA_ARGS__)

#define MBLTRC_TRACE_HANDLE_WARN(handle, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::warn, \
        handle, \
        __VA_ARGS__)

#define MBLTRC_TRACE_HANDLE_ERROR(handle, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::error, \
        handle, \
        __VA_ARGS__)

#define MBLTRC_TRACE_HANDLE_FATAL(handle, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::fatal, \
        handle, \
        __VA_ARGS__)

#define MBLTRC_TRACE_HANDLE_ALWAYS(handle, ...) \
    MBLTRC_TRACE(__FILE__, \
        __LINE__, \
        melinda::mbltrc::trace_level::always, \
        handle, \
        __VA_ARGS__)

#define MBLTRC_TRACE_DEBUG(...) \
    MBLTRC_TRACE_HANDLE_DEBUG(melinda::mbltrc::process_trace_handle(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_INFO(...) \
    MBLTRC_TRACE_HANDLE_INFO(melinda::mbltrc::process_trace_handle(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_WARN(...) \
    MBLTRC_TRACE_HANDLE_WARN(melinda::mbltrc::process_trace_handle(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_ERROR(...) \
    MBLTRC_TRACE_HANDLE_ERROR(melinda::mbltrc::process_trace_handle(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_FATAL(...) \
    MBLTRC_TRACE_HANDLE_FATAL(melinda::mbltrc::process_trace_handle(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_ALWAYS(...) \
    MBLTRC_TRACE_HANDLE_ALWAYS(melinda::mbltrc::process_trace_handle(), \
        __VA_ARGS__)

#endif
