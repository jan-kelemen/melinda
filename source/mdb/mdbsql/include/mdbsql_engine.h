#ifndef MELINDA_MDBSQL_ENGINE_INCLUDED
#define MELINDA_MDBSQL_ENGINE_INCLUDED

#include <string>
#include <string_view>
#include <vector>

#include <mblcxx_sops.h>

namespace melinda::mdbsql
{
    class [[nodiscard]] engine final : public mblcxx::sops::noncopyable
    {
    public: // Construction
        engine() = default;

    public: // Destruction
        ~engine() = default;

    public: // Interface
        bool execute(std::string_view query);

    private: // Data
        std::vector<std::string> databases_;
    };
} // namespace melinda::mdbsql

#endif
