#ifndef MELINDA_MBLTRC_TRACE_SINK_INCLUDED
#define MELINDA_MBLTRC_TRACE_SINK_INCLUDED

#include <string_view>
#include <thread>

#include <mbltrc_timestamp.h>
#include <mbltrc_trace_level.h>

namespace melinda::mbltrc
{
    struct message_origin final
    {
        std::thread::id thread_id;
        std::string_view file;
        uint_least32_t line;
    };

    class [[nodiscard]] sink
    {
    public: // Interface
        virtual bool should_trace_message(
            trace_level message_level) const noexcept = 0;

        virtual void change_level(trace_level new_level) noexcept = 0;

        virtual void trace(message_origin const& origin,
            timestamp const& time,
            trace_level level,
            std::string_view message) const noexcept = 0;

    public: // Destruction
        virtual ~sink() noexcept;
    };
} // namespace melinda::mbltrc

#endif
