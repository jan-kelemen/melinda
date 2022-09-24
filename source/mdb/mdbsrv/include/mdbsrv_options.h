#ifndef MELINDA_MDBSRV_OPTIONS_INCLUDED
#define MELINDA_MDBSRV_OPTIONS_INCLUDED

#include <filesystem>
#include <optional>

namespace melinda::mdbsrv
{
    struct [[nodiscard]] options
    {
        std::filesystem::path data_directory;
    };

    [[nodiscard]] std::optional<options> parse_options(int argc, char** argv);
} // namespace melinda::mdbsrv

#endif
