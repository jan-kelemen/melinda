#ifndef MELINDA_MBLTRC_TRACE_OPTIONS_INCLUDED
#define MELINDA_MBLTRC_TRACE_OPTIONS_INCLUDED

#include <cstdint>
#include <filesystem>

#include <mbltrc_trace_level.h>

namespace melinda::mbltrc
{
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
} // namespace melinda::mbltrc

#endif
