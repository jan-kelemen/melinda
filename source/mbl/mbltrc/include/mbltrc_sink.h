#ifndef MELINDA_MBLTRC_TRACE_SINK_INCLUDED
#define MELINDA_MBLTRC_TRACE_SINK_INCLUDED

#include <source_location>
#include <string_view>
#include <thread>

#include <mbltrc_timestamp.h>
#include <mbltrc_trace_level.h>

namespace melinda::mbltrc
{
    struct [[nodiscard]] message_origin final
    {
        std::source_location source;
        std::thread::id thread_id;
    };

    class [[nodiscard]] sink
    {
    public: // Interface
        virtual void change_level(trace_level new_level) noexcept = 0;

        [[nodiscard]] virtual bool should_trace_message(
            trace_level level) const noexcept = 0;

        virtual void trace(message_origin const& origin,
            timestamp const& time,
            trace_level level,
            std::string_view message) const noexcept = 0;

    public: // Destruction
        virtual ~sink() noexcept = default;
    };
} // namespace melinda::mbltrc

#endif
