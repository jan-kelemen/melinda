#include <mbltrc_trace.h>

namespace melinda::mbltrc
{
    class sink;
} // namespace melinda::mbltrc

namespace
{
    std::weak_ptr<melinda::mbltrc::sink> proc_sink;
} // namespace

std::shared_ptr<melinda::mbltrc::sink> melinda::mbltrc::detail::process_sink()
{
    return proc_sink.lock();
}

void melinda::mbltrc::register_process_sink(
    std::weak_ptr<melinda::mbltrc::sink> sink)
{
    proc_sink = sink;
}
