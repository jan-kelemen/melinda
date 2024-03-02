#ifndef MELINDA_MDBXTR_EXECUTE_INCLUDED
#define MELINDA_MDBXTR_EXECUTE_INCLUDED

#include <mblcxx_expected.hpp>

#include <cstdint>
#include <string_view>

namespace melinda::mdbxtr
{
    mblcxx::expected<std::uint64_t, std::error_code> execute(
        std::string_view sql_text);
} // namespace melinda::mdbxtr

#endif
