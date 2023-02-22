#ifndef MELINDA_MBLTRC_STREAM_SINK_INCLUDED
#define MELINDA_MBLTRC_STREAM_SINK_INCLUDED

#include <functional>
#include <iosfwd>
#include <mutex>

#include <mbltrc_sink.h>

namespace melinda::mbltrc
{
    class [[nodiscard]] stream_sink : public sink
    {
    public: // Construction
        stream_sink(trace_level initial_level,
            std::reference_wrapper<std::ostream> stream);

    public: // Interface
        bool should_trace_message(
            trace_level message_level) const noexcept override;

        void change_level(trace_level new_level) noexcept override;

        void trace(message_origin const& origin,
            timestamp const& time,
            trace_level level,
            std::string_view message) const noexcept override;

    public: // Destruction
        ~stream_sink() noexcept;

    private: // Data
        mutable std::atomic<trace_level> current_level_;
        mutable std::timed_mutex mtx_;
        mutable std::reference_wrapper<std::ostream> stream_;
    };
} // namespace melinda::mbltrc

#endif
