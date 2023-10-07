#ifndef MELINDA_MDBSQL_ENGINE_INCLUDED
#define MELINDA_MDBSQL_ENGINE_INCLUDED

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace melinda::mdbsql
{
    class [[nodiscard]] engine final
    {
    public: // Construction
        engine(std::filesystem::path data_directory);

        engine(engine const&) = delete;

    public: // Destruction
        ~engine() = default;

    public: // Interface
        bool execute(std::string_view query);

    public: // Operators
        engine& operator=(engine const&) = delete;

    private: // Data
        std::filesystem::path data_directory_;
        std::vector<std::string> databases_;
    };
} // namespace melinda::mdbsql

#endif
