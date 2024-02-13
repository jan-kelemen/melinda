#ifndef MELINDA_MQLPRS_TREE_UTIL_HPP
#define MELINDA_MQLPRS_TREE_UTIL_HPP

#include <lexy/grammar.hpp>

#include <ranges>
#include <string_view>

namespace melinda::mqlprs
{
    struct tree_util final
    {
        template<typename Production, typename Node>
        [[nodiscard]] constexpr static auto children_of_type(Node const& node)
        {
            return node.children() |
                std::views::filter(
                    [](auto&& child)
                    {
                        return std::string_view{child.kind().name()} ==
                            lexy::production_name<Production>();
                    });
        }
    };
} // namespace melinda::mqlprs

#endif
