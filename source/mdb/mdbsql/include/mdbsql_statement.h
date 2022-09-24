#ifndef MELINDA_MDBSQL_STATEMENT_INCLUDED
#define MELINDA_MDBSQL_STATEMENT_INCLUDED

#include <string>
#include <variant>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>

namespace melinda::mdbsql
{
    struct [[nodiscard]] statement final
    {
        struct [[nodiscard]] create final
        {
            std::string database_name;
        };

        boost::variant<create> value;
    };
} // namespace melinda::mdbsql

BOOST_FUSION_ADAPT_STRUCT(melinda::mdbsql::statement::create, database_name)
BOOST_FUSION_ADAPT_STRUCT(melinda::mdbsql::statement, value)

#endif
