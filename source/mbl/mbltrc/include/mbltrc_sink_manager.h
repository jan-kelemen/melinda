#ifndef MELINDA_MBLTRC_SINK_MANAGER_INCLUDED
#define MELINDA_MBLTRC_SINK_MANAGER_INCLUDED

#include <initializer_list>
#include <memory>
#include <mutex>
#include <vector>

#include <mbltrc_sink.h>

namespace melinda::mbltrc
{
    struct [[nodiscard]] sink_manager_guard final
    {
    public: // Construction
        sink_manager_guard();

        sink_manager_guard(sink_manager_guard const&) = delete;

        sink_manager_guard(sink_manager_guard&&) noexcept = delete;

    public: // Interface
        sink_manager_guard& operator=(sink_manager_guard const&) = delete;

        sink_manager_guard& operator=(sink_manager_guard&&) noexcept = delete;

    public:
        ~sink_manager_guard() noexcept;
    };

    class [[nodiscard]] sink_manager final
    {
    public: // Static interface
        static std::weak_ptr<sink_manager> instance();

    public: // Construction
        sink_manager(sink_manager const&) = delete;

        sink_manager(sink_manager&&) noexcept = delete;

    public: // Interface
        void add(std::shared_ptr<sink> sink);

        void remove(std::shared_ptr<sink> const& sink);

        void trace(message_origin const& origin,
            timestamp const& time,
            trace_level level,
            std::string_view message) const noexcept;

    public: // Operators
        sink_manager& operator=(sink_manager const&) = delete;

        sink_manager& operator=(sink_manager&&) noexcept = delete;

    private: // Construction
        sink_manager() = default;

    private: // Destruction
        ~sink_manager() noexcept = default;

    private: // Data
        mutable std::mutex mtx_;
        std::vector<std::shared_ptr<sink>> sinks_;
    };
} // namespace melinda::mbltrc

#endif
