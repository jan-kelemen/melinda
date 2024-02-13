#ifndef MELINDA_MQLAST_FROM_PARSE_TREE_INCLUDED
#define MELINDA_MQLAST_FROM_PARSE_TREE_INCLUDED

#include <mqlast_syntax_tree.hpp>

#include <mqlprs_parse.hpp>
#include <mqlprs_tree_util.hpp>

#include <mblcxx_result.hpp>

namespace melinda::mqlast
{
    template<typename T>
    [[nodiscard]] mblcxx::result<syntax_tree> from_parse_tree(
        T const& parse_tree)
    {
        syntax_tree rv;

        for (auto const& child :
            mqlprs::tree_util::children_of_type<mqlprs::create_statement>(
                parse_tree.root()))
        {
            auto identifiers{
                mqlprs::tree_util::children_of_type<mqlprs::identifier>(child)};
            assert(std::distance(identifiers.begin(), identifiers.end()) == 1);

            auto const& identifier{identifiers.begin()->lexeme()};
            rv.commands.emplace_back(
                create_schema_command{{identifier.begin(), identifier.end()}});
        }

        return rv;
    }
} // namespace melinda::mqlast

#endif
