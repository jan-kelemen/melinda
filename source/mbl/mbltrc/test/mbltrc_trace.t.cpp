#include <mbltrc_trace.h>

using namespace melinda;
using namespace mbltrc;

static_assert(trace_level::debug < trace_level::always);
static_assert(trace_level::info < trace_level::always);
static_assert(trace_level::warn < trace_level::always);
static_assert(trace_level::error < trace_level::always);
static_assert(trace_level::fatal < trace_level::always);
