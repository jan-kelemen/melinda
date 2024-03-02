#ifndef MELINDA_MQLAST_FROM_PARSE_TREE_INCLUDED
#define MELINDA_MQLAST_FROM_PARSE_TREE_INCLUDED

#include <mqlast_syntax_tree.hpp>

#include <mqlprs_parse.hpp>
#include <mqlprs_tree_util.hpp>

#include <mblcxx_expected.hpp>

#include <cassert>
#include <system_error>

namespace melinda::mqlast
{
    template<typename T>
    [[nodiscard]] mblcxx::expected<syntax_tree, std::error_code>
    from_parse_tree(T const& parse_tree)
    {
        syntax_tree rv;

        if (parse_tree.empty())
        {
            return rv;
        }

        for (auto const& child :
            mqlprs::tree_util::children_of_type<mqlprs::create_statement>(
                parse_tree.root()))
        {
            auto identifiers{
                mqlprs::tree_util::children_of_type<mqlprs::identifier>(child)};
            assert(std::distance(identifiers.begin(), identifiers.end()) == 1);

            auto const& identifier{
                identifiers.begin()->children().begin()->lexeme()};
            rv.commands.emplace_back(
                create_schema_command{{identifier.begin(), identifier.end()}});
        }

        return rv;
    }
} // namespace melinda::mqlast

#endif
