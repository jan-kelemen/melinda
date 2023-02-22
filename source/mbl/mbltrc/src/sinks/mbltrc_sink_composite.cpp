#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <mbltrc_sink_composite.h>

melinda::mbltrc::sink_composite::sink_composite() = default;

melinda::mbltrc::sink_composite::sink_composite(
    melinda::mbltrc::sink_composite&& other) noexcept
{
    std::lock_guard g{other.mtx_};
    sinks_ = std::move(other.sinks_);
}

melinda::mbltrc::sink_composite::sink_composite(
    std::initializer_list<std::shared_ptr<melinda::mbltrc::sink>> init)
    : sinks_{init}
{
}

void melinda::mbltrc::sink_composite::add(
    std::shared_ptr<melinda::mbltrc::sink> sink)
{
    std::lock_guard g{mtx_};
    sinks_.push_back(std::move(sink));
}

void melinda::mbltrc::sink_composite::remove(sink* sink)
{
    std::lock_guard g{mtx_};
    std::erase_if(sinks_,
        [sink](auto const& s) noexcept { return s.get() == sink; });
}

bool melinda::mbltrc::sink_composite::should_trace_message(
    melinda::mbltrc::trace_level message_level) const noexcept
{
    std::lock_guard g{mtx_};
    return std::any_of(std::cbegin(sinks_),
        std::cend(sinks_),
        [message_level](auto const& sink)
        { return sink->should_trace_message(message_level); });
}

void melinda::mbltrc::sink_composite::change_level(
    melinda::mbltrc::trace_level new_level) noexcept
{
    std::lock_guard g{mtx_};
    std::for_each(std::cbegin(sinks_),
        std::cend(sinks_),
        [new_level](auto& sink) { sink->change_level(new_level); });
}

void melinda::mbltrc::sink_composite::trace(
    melinda::mbltrc::message_origin const& origin,
    melinda::mbltrc::timestamp const& time,
    melinda::mbltrc::trace_level level,
    std::string_view message) const noexcept
{
    std::vector<std::shared_ptr<sink>> effective_sinks;
    effective_sinks.reserve(sinks_.size());

    {
        std::lock_guard g{mtx_};
        std::copy_if(std::cbegin(sinks_),
            std::cend(sinks_),
            std::back_inserter(effective_sinks),
            [level](auto const& sink)
            { return sink->should_trace_message(level); });
    }

    std::for_each(std::cbegin(effective_sinks),
        std::cend(effective_sinks),
        [&origin, &time, &level, &message](auto const& sink)
        { sink->trace(origin, time, level, message); });
}

melinda::mbltrc::sink_composite& melinda::mbltrc::sink_composite::operator=(
    melinda::mbltrc::sink_composite&& other) noexcept
{
    std::lock_guard g{other.mtx_};
    sinks_ = std::move(other.sinks_);

    return *this;
}

melinda::mbltrc::sink_composite::~sink_composite() noexcept = default;
