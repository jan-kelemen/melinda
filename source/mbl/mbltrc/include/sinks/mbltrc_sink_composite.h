#ifndef MELINDA_MBLTRC_SINK_COMPOSITE_INCLUDED
#define MELINDA_MBLTRC_SINK_COMPOSITE_INCLUDED

#include <initializer_list>
#include <memory>
#include <mutex>
#include <vector>

#include <mbltrc_sink.h>

namespace melinda::mbltrc
{
    class [[nodiscard]] sink_composite : public sink
    {
    public: // Construction
        sink_composite();

        sink_composite(std::initializer_list<std::shared_ptr<sink>> init);

        sink_composite(sink_composite const&) = delete;

        sink_composite(sink_composite&&) noexcept;

    public: // Interface
        void add(std::shared_ptr<sink> sink);

        void remove(sink* sink);

    public: // sink overrides
        bool should_trace_message(
            trace_level message_level) const noexcept override;

        void change_level(trace_level new_level) noexcept override;

        void trace(message_origin const& origin,
            timestamp const& time,
            trace_level level,
            std::string_view message) const noexcept override;

    public: // Operators
        sink_composite& operator=(sink_composite const&) = delete;

        sink_composite& operator=(sink_composite&&) noexcept;

    public: // Destruction
        ~sink_composite() noexcept;

    private: // Data
        mutable std::mutex mtx_;
        std::vector<std::shared_ptr<sink>> sinks_;
    };
} // namespace melinda::mbltrc

#endif
