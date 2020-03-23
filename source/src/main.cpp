#include "../common/trace/include/trace.h"

#include <iostream>

int main()
{
    melinda::trace::trace_options options(fs::path("../log/"),
        fs::path("tryout"));
    melinda::trace::trace_handle handle =
        melinda::trace::initialize_trace(options);

    for (int i = 0; i < 10000; ++i)
    {
        MELINDA_TRACE_ERROR(handle, "{}", i);
    }

    melinda::trace::close_trace(handle);
}
