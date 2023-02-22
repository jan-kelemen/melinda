#include <mbltrc_stream_sink.h>

#include <chrono>

#include <fmt/std.h>

#include <mblcxx_scope_exit.h>

melinda::mbltrc::stream_sink::stream_sink(trace_level initial_level,
    std::reference_wrapper<std::ostream> stream)
    : current_level_{initial_level}
    , stream_{stream}
{
}

bool melinda::mbltrc::stream_sink::should_trace_message(
    trace_level message_level) const noexcept
{
    return message_level >= current_level_.load(std::memory_order_acquire);
}

void melinda::mbltrc::stream_sink::change_level(trace_level new_level) noexcept
{
    current_level_.store(new_level, std::memory_order_release);
}

void melinda::mbltrc::stream_sink::trace(
    melinda::mbltrc::message_origin const& origin,
    melinda::mbltrc::timestamp const& time,
    melinda::mbltrc::trace_level level,
    std::string_view message) const noexcept
{
    using namespace std::chrono_literals;
    if (!mtx_.try_lock_for(100ms))
    {
        return;
    }
    MBLCXX_ON_SCOPE_EXIT(mtx_.unlock());

    stream_.get() << fmt::format("{};{};{};{};{}:{}\n",
        time,
        level,
        origin.thread_id,
        message,
        origin.file,
        origin.line);
}

melinda::mbltrc::stream_sink::~stream_sink() noexcept = default;
