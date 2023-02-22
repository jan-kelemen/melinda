#include <mbltrc_trace_options.h>

melinda::mbltrc::trace_options::trace_options(std::filesystem::path directory,
    std::filesystem::path filename)
    : trace_directory(std::move(directory))
    , base_filename(std::move(filename))
{
}
