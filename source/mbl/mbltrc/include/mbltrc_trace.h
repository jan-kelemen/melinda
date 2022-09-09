#ifndef MELINDA_MBLTRC_TRACE_INCLUDED
#define MELINDA_MBLTRC_TRACE_INCLUDED

#include <cstdint>
#include <filesystem>
#include <limits>
#include <memory>
#include <string_view>
#include <type_traits>

#include <fmt/format.h>

// Provides trace support for the application module

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

        trace_options(std::filesystem::path directory,
            std::filesystem::path filename);

        trace_options(trace_options const&) = default;

        trace_options(trace_options&&) noexcept = default;

    public: // Operators
        trace_options& operator=(trace_options const&) = default;

        trace_options& operator=(trace_options&&) noexcept = default;

    public: // Destruction
        ~trace_options() noexcept = default;

    public: // Data
        // Directory used for tracing
        std::filesystem::path trace_directory =
            std::filesystem::current_path() / "../log";

        // Base of the filename used for trace files
        std::filesystem::path base_filename =
            std::filesystem::path("melinda_trace");

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

        trace_handle(trace_handle&&) noexcept = default;

    public: // Interface
        [[nodiscard]] bool should_trace_message(
            trace_level level) const noexcept;

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

} // namespace melinda::mbltrc

namespace melinda::mbltrc::detail
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
} // namespace melinda::mbltrc::detail

template<>
struct fmt::formatter<melinda::mbltrc::detail::timestamp>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mbltrc::detail::timestamp const& t, FormatContext& ctx)
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
} // namespace melinda::mbltrc::detail

#define MBLTRC_TRACE(f, l, level, handle, ...) \
    do \
    { \
        std::string_view filename_b015cf7e_932f_4d99_822a_b863763c6e23 = \
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
    MBLTRC_TRACE_HANDLE_DEBUG(melinda::mbltrc::detail::process_trace_handle(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_INFO(...) \
    MBLTRC_TRACE_HANDLE_INFO(melinda::mbltrc::detail::process_trace_handle(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_WARN(...) \
    MBLTRC_TRACE_HANDLE_WARN(melinda::mbltrc::detail::process_trace_handle(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_ERROR(...) \
    MBLTRC_TRACE_HANDLE_ERROR(melinda::mbltrc::detail::process_trace_handle(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_FATAL(...) \
    MBLTRC_TRACE_HANDLE_FATAL(melinda::mbltrc::detail::process_trace_handle(), \
        __VA_ARGS__)

#define MBLTRC_TRACE_ALWAYS(...) \
    MBLTRC_TRACE_HANDLE_ALWAYS( \
        melinda::mbltrc::detail::process_trace_handle(), \
        __VA_ARGS__)

#endif
