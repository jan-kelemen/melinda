#ifndef MELINDA_COMMON_CPPEX_TRACE_TRACE_INCLUDED
#define MELINDA_COMMON_CPPEX_TRACE_TRACE_INCLUDED

#include <cstdint>
#include <filesystem>
#include <fmt/format.h>
#include <limits>
#include <memory>
#include <string_view>
#include <type_traits>

namespace fs = std::filesystem;

// Provides trace support for the application module

namespace mel::trace
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
    [[nodiscard]] trace_handle create_trace_handle(
        trace_options const& options);

    // Initialize process wide tracing handle
    void initialize_process_trace_handle(trace_handle&& handle);

    // Closes the trace handle
    void close_trace(trace_handle& handle) noexcept;

} // namespace mel::trace

namespace mel::trace::detail
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
} // namespace mel::trace::detail

template<>
struct fmt::formatter<mel::trace::detail::timestamp>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(mel::trace::detail::timestamp const& t, FormatContext& ctx)
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
struct fmt::formatter<mel::trace::trace_level>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(mel::trace::trace_level const& l, FormatContext& ctx)
    {
        char const marker = [&l]() noexcept {
            switch (l)
            {
            case mel::trace::trace_level::debug:
                return 'D';
            case mel::trace::trace_level::info:
                return 'I';
            case mel::trace::trace_level::warn:
                return 'W';
            case mel::trace::trace_level::error:
                return 'E';
            case mel::trace::trace_level::fatal:
                return 'F';
            case mel::trace::trace_level::always:
                return 'A';
            default:
                return 'X';
            }
        }();

        return format_to(ctx.out(), "[{}]", marker);
    }
};

namespace mel::trace::detail
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
        mel::trace::trace_level const level,
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

    [[nodiscard]] trace_handle const* process_trace_handle() noexcept;

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
} // namespace mel::trace::detail

#define MEL_TRACE(f, l, level, handle, ...) \
    do \
    { \
        constexpr char const* const \
            filename_b015cf7e_932f_4d99_822a_b863763c6e23 = \
                mel::trace::detail::file_name_only(f); \
        if (mel::trace::detail::should_trace_message(handle, level)) \
        { \
            mel::trace::detail::trace(handle, \
                mel::trace::detail::format_message( \
                    filename_b015cf7e_932f_4d99_822a_b863763c6e23, \
                    l, \
                    level, \
                    __VA_ARGS__)); \
        } \
    } while (false)

#define MEL_TRACE_HANDLE_DEBUG(handle, ...) \
    MEL_TRACE(__FILE__, \
        __LINE__, \
        mel::trace::trace_level::debug, \
        handle, \
        __VA_ARGS__)

#define MEL_TRACE_HANDLE_INFO(handle, ...) \
    MEL_TRACE(__FILE__, \
        __LINE__, \
        mel::trace::trace_level::info, \
        handle, \
        __VA_ARGS__)

#define MEL_TRACE_HANDLE_WARN(handle, ...) \
    MEL_TRACE(__FILE__, \
        __LINE__, \
        mel::trace::trace_level::warn, \
        handle, \
        __VA_ARGS__)

#define MEL_TRACE_HANDLE_ERROR(handle, ...) \
    MEL_TRACE(__FILE__, \
        __LINE__, \
        mel::trace::trace_level::error, \
        handle, \
        __VA_ARGS__)

#define MEL_TRACE_HANDLE_FATAL(handle, ...) \
    MEL_TRACE(__FILE__, \
        __LINE__, \
        mel::trace::trace_level::fatal, \
        handle, \
        __VA_ARGS__)

#define MEL_TRACE_HANDLE_ALWAYS(handle, ...) \
    MEL_TRACE(__FILE__, \
        __LINE__, \
        mel::trace::trace_level::always, \
        handle, \
        __VA_ARGS__)

#define MEL_TRACE_DEBUG(...) \
    MEL_TRACE_HANDLE_DEBUG(mel::trace::detail::process_trace_handle(), \
        __VA_ARGS__)

#define MEL_TRACE_INFO(...) \
    MEL_TRACE_HANDLE_INFO(mel::trace::detail::process_trace_handle(), \
        __VA_ARGS__)

#define MEL_TRACE_WARN(...) \
    MEL_TRACE_HANDLE_WARN(mel::trace::detail::process_trace_handle(), \
        __VA_ARGS__)

#define MEL_TRACE_ERROR(...) \
    MEL_TRACE_HANDLE_ERROR(mel::trace::detail::process_trace_handle(), \
        __VA_ARGS__)

#define MEL_TRACE_FATAL(...) \
    MEL_TRACE_HANDLE_FATAL(mel::trace::detail::process_trace_handle(), \
        __VA_ARGS__)

#define MEL_TRACE_ALWAYS(...) \
    MEL_TRACE_HANDLE_ALWAYS(mel::trace::detail::process_trace_handle(), \
        __VA_ARGS__)

#endif
