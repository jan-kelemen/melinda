#ifndef MELINDA_MBLTRC_MEMORY_MAPPED_SINK_INCLUDED
#define MELINDA_MBLTRC_MEMORY_MAPPED_SINK_INCLUDED

#include <filesystem>
#include <memory>

#include <mbltrc_sink.h>

namespace melinda::mbltrc
{
    class [[nodiscard]] memory_mapped_sink final : public sink
    {
    public: // Construction
        memory_mapped_sink(trace_level initial_level,
            std::filesystem::path const& directory,
            std::filesystem::path const& base_filename,
            std::size_t file_size);

        memory_mapped_sink(memory_mapped_sink const&) = delete;

        memory_mapped_sink(memory_mapped_sink&&) noexcept = default;

    public: // Interface
        void change_level(trace_level new_level) noexcept override;

        [[nodiscard]] bool should_trace_message(
            trace_level level) const noexcept override;

        void trace(message_origin const& origin,
            timestamp const& time,
            trace_level level,
            std::string_view message) const noexcept override;

    public: // Operators
        memory_mapped_sink& operator=(memory_mapped_sink const&) = delete;

        memory_mapped_sink& operator=(memory_mapped_sink&&) noexcept = default;

    public: // Destruction
        ~memory_mapped_sink() noexcept;

    private: // Data
        class impl;
        std::unique_ptr<impl> impl_;
    };
} // namespace melinda::mbltrc
#endif
