#include "mppci.h"
#include "trace.h"

int main()
{
    mel::trace::trace_options trace_config(fs::path("../log"),
        fs::path("example_client"));
    trace_config.level = mel::trace::trace_level::info;

    mel::trace::initialize_process_trace_handle(
        mel::trace::create_trace_handle(trace_config));

    mel::mppci::old_main();
}
