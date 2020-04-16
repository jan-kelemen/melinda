#include "common/cppex/include/scope_exit.h"
#include "common/trace/include/trace.h"

#include <iostream>

int main()
{
    melinda::trace::trace_options const options(fs::path("../log/"),
        fs::path("tryout"));
    melinda::trace::trace_handle handle =
        melinda::trace::initialize_trace(options);
    ON_SCOPE_EXIT(melinda::trace::close_trace(handle));

    for (int i = 0; i < 10000; ++i)
    {
        MELINDA_TRACE_ERROR(handle, "{}", i);
    }
}
