#ifndef MELINDA_COMMON_CPPEX_ITERATOR_TRAITS_INCLUDED
#define MELINDA_COMMON_CPPEX_ITERATOR_TRAITS_INCLUDED

#include <iterator>
#include <type_traits>

// Provides utility traits for checking iterator categories

namespace mel::cppex
{
    template<typename Iterator>
    using iterator_category =
        typename std::iterator_traits<Iterator>::iterator_category;

    namespace detail
    {
        template<typename Iterator, typename Category>
        struct is_of_iterator_category final
            : std::bool_constant<
                  std::is_convertible_v<iterator_category<Iterator>, Category>>
        {
        };
    } // namespace detail

    template<typename Iterator, typename Category>
    using is_of_iterator_category =
        detail::is_of_iterator_category<Iterator, Category>;

    template<typename Iterator, typename Category>
    inline constexpr bool is_of_iterator_category_v =
        is_of_iterator_category<Iterator, Category>::value;

    template<typename Iterator>
    using is_input_iterator =
        is_of_iterator_category<Iterator, std::input_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_input_iterator_v =
        is_input_iterator<Iterator>::value;

    template<typename Iterator>
    concept input_iterator = std::input_iterator<Iterator>;

    template<typename Iterator>
    using is_forward_iterator =
        is_of_iterator_category<Iterator, std::forward_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_forward_iterator_v =
        is_forward_iterator<Iterator>::value;

    template<typename Iterator>
    concept forward_iterator = std::forward_iterator<Iterator>;

    template<typename Iterator>
    using is_bidirectional_iterator =
        is_of_iterator_category<Iterator, std::bidirectional_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_bidirectional_iterator_v =
        is_bidirectional_iterator<Iterator>::value;

    template<typename Iterator>
    concept bidirectional_iterator = std::bidirectional_iterator<Iterator>;

    template<typename Iterator>
    using is_random_access_iterator =
        is_of_iterator_category<Iterator, std::random_access_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_random_access_iterator_v =
        is_random_access_iterator<Iterator>::value;

    template<typename Iterator>
    concept random_access_iterator = std::random_access_iterator<Iterator>;

    template<typename Iterator>
    using is_contiguous_iterator =
        is_of_iterator_category<Iterator, std::contiguous_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_contiguous_iterator_v =
        is_contiguous_iterator<Iterator>::value;

    template<typename Iterator>
    concept contiguous_iterator = std::contiguous_iterator<Iterator>;

    template<typename Iterator>
    using is_output_iterator =
        is_of_iterator_category<Iterator, std::output_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_output_iterator_v =
        is_output_iterator<Iterator>::value;

    template<typename Iterator, typename Value>
    concept output_iterator = std::output_iterator<Iterator, Value>;
} // namespace mel::cppex

#endif
