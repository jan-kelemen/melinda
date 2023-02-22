#ifndef MELINDA_MBLTRC_TRACE_HANDLE_INCLUDED
#define MELINDA_MBLTRC_TRACE_HANDLE_INCLUDED

#include <memory>
#include <string_view>

#include <mbltrc_trace_level.h>
#include <mbltrc_trace_options.h>

namespace melinda::mbltrc
{
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

        void close() noexcept;

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

    [[nodiscard]] trace_handle const* process_trace_handle() noexcept;
} // namespace melinda::mbltrc

#endif
