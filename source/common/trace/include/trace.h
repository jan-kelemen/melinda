#ifndef MELINDA_COMMON_CPPEX_TRACE_TRACE_INCLUDED
#define MELINDA_COMMON_CPPEX_TRACE_TRACE_INCLUDED

#include "boost/filesystem.hpp"
#include "fmt/format.h"

#include <cstdint>
#include <limits>
#include <memory>
#include <string_view>
#include <type_traits>

// Provides trace support for the application module

namespace fs = boost::filesystem;

namespace melinda::trace
{
    enum class [[nodiscard]] trace_level : uint8_t {
        debug = 1,
        info = 2,
        warn = 3,
        error = 4,
        fatal = 5,
        always =
            std::numeric_limits<std::underlying_type_t<trace_level>>::max(),
    };

    // Configuration options of the trace library
    struct [[nodiscard]] trace_options final
    {
    public: // Types
        using class_name = trace_options;

    public: // Constants
        static constexpr trace_level default_level = trace_level::error;
        static constexpr uintmax_t default_max_filesize = 10 * 1024;
        static constexpr uint8_t default_preserved_files = 10;
        static constexpr bool default_flush_on_write = true;

    public: // Construction
        trace_options() = default;

        trace_options(fs::path directory, fs::path filename);

        trace_options(trace_options const&) = default;

        trace_options(trace_options &&) noexcept = default;

    public: // Operators
        trace_options& operator=(trace_options const&) = default;

        trace_options& operator=(trace_options&&) noexcept = default;

    public: // Destruction
        ~trace_options() noexcept = default;

    public: // Data
        // Directory used for tracing
        fs::path trace_directory = fs::current_path() / "../log";

        // Base of the filename used for trace files
        fs::path base_filename = fs::path("melinda_trace");

        // Minimum level of the messages which will be traced
        trace_level level = default_level;

        // Limit the size of the trace files in bytes. Set to 0 for unlimited
        // size.
        uintmax_t max_filesize = default_max_filesize;

        // Limit the count of trace files matching base name pattern.
        // Set to 0 for unlimited preserved files.
        uint8_t log_files_to_keep = default_preserved_files;

        // Setting if the trace will be flushed to target on each message.
        bool flush_on_write = default_flush_on_write;
    };

    // Handle to the trace for the trace client to use.
    class [[nodiscard]] trace_handle final
    {
    public: // Types
        using class_name = trace_handle;

    public: // Construction
        trace_handle() noexcept;

        trace_handle(trace_options const& options);

        trace_handle(trace_handle const&) noexcept = default;

        trace_handle(trace_handle &&) noexcept = default;

    public: // Interface
        [[nodiscard]] bool should_trace_message(trace_level level)
            const noexcept;

        void trace_level(trace_level new_level) noexcept;

        void trace(std::string_view message) const noexcept;

    public: // Operators
        trace_handle& operator=(trace_handle const&) noexcept = default;

        trace_handle& operator=(trace_handle&&) noexcept = default;

    public: // Conversions
        [[nodiscard]] explicit operator bool() const noexcept;

    public: // Destruction
        ~trace_handle() noexcept;

    private: // Types
        class impl;

    private: // Data
        std::shared_ptr<impl> impl_;
    };

    // Provides the handle for the tracing
    [[nodiscard]] trace_handle initialize_trace(trace_options const& options);

    // Closes the trace handle
    void close_trace(trace_handle& handle) noexcept;

} // namespace melinda::trace

namespace melinda::trace::detail
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
} // namespace melinda::trace::detail

template<>
struct fmt::formatter<melinda::trace::detail::timestamp>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::trace::detail::timestamp const& t, FormatContext& ctx)
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

template<>
struct fmt::formatter<melinda::trace::trace_level>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::trace::trace_level const& l, FormatContext& ctx)
    {
        char const marker = [&l]() noexcept {
            switch (l)
            {
            case melinda::trace::trace_level::debug:
                return 'D';
            case melinda::trace::trace_level::info:
                return 'I';
            case melinda::trace::trace_level::warn:
                return 'W';
            case melinda::trace::trace_level::error:
                return 'E';
            case melinda::trace::trace_level::fatal:
                return 'F';
            case melinda::trace::trace_level::always:
                return 'A';
            default:
                return 'X';
            }
        }();

        return format_to(ctx.out(), "[{}]", marker);
    }
};

namespace melinda::trace::detail
{
    // https://stackoverflow.com/a/29921055
    constexpr char const* strend(char const* const str)
    {
        return *str ? strend(str + 1) : str;
    }

    constexpr char const* from_last_slash(char const* const start,
        char const* const end)
    {
        return (end >= start && *end != '/' && *end != '\\')
            ? from_last_slash(start, end - 1)
            : (end + 1);
    }

    constexpr char const* file_name_only(char const* const path)
    {
        return from_last_slash(path, strend(path));
    }

    template<typename... T>
    [[nodiscard]] std::string format_message(char const* const filename,
        uintmax_t const line,
        melinda::trace::trace_level const level,
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
        return fmt::format(real_format, std::forward<T>(args)...);
    }
} // namespace melinda::trace::detail

#define MELINDA_TRACE(f, l, level, handle, format, ...) \
    do \
    { \
        constexpr char const* const \
            filename_b015cf7e_932f_4d99_822a_b863763c6e23 = \
                melinda::trace::detail::file_name_only(f); \
        if (handle.should_trace_message(level)) \
        { \
            handle.trace(melinda::trace::detail::format_message( \
                filename_b015cf7e_932f_4d99_822a_b863763c6e23, \
                l, \
                level, \
                format, \
                __VA_ARGS__)); \
        } \
    } while (false)

#define MELINDA_TRACE_DEBUG(handle, format, ...) \
    MELINDA_TRACE(__FILE__, \
        __LINE__, \
        melinda::trace::trace_level::debug, \
        handle, \
        format, \
        __VA_ARGS__)

#define MELINDA_TRACE_INFO(handle, format, ...) \
    MELINDA_TRACE(__FILE__, \
        __LINE__, \
        melinda::trace::trace_level::info, \
        handle, \
        format, \
        __VA_ARGS__)

#define MELINDA_TRACE_WARN(handle, format, ...) \
    MELINDA_TRACE(__FILE__, \
        __LINE__, \
        melinda::trace::trace_level::warn, \
        handle, \
        format, \
        __VA_ARGS__)

#define MELINDA_TRACE_ERROR(handle, format, ...) \
    MELINDA_TRACE(__FILE__, \
        __LINE__, \
        melinda::trace::trace_level::error, \
        handle, \
        format, \
        __VA_ARGS__)

#define MELINDA_TRACE_FATAL(handle, format, ...) \
    MELINDA_TRACE(__FILE__, \
        __LINE__, \
        melinda::trace::trace_level::fatal, \
        handle, \
        format, \
        __VA_ARGS__)

#define MELINDA_TRACE_ALWAYS(handle, format, ...) \
    MELINDA_TRACE(__FILE__, \
        __LINE__, \
        melinda::trace::trace_level::always, \
        handle, \
        format, \
        __VA_ARGS__)

#endif
