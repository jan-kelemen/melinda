#ifndef MELINDA_MDBXTR_EXECUTE_INCLUDED
#define MELINDA_MDBXTR_EXECUTE_INCLUDED

#include <mblcxx_result.hpp>

#include <cstdint>
#include <string_view>

namespace melinda::mdbxtr
{
    mblcxx::result<std::uint64_t> execute(std::string_view sql_text);
} // namespace melinda::mdbxtr

#endif
