#ifndef MELINDA_COMMON_CPPEX_ITERATOR_TRAITS_INCLUDED
#define MELINDA_COMMON_CPPEX_ITERATOR_TRAITS_INCLUDED

#include <iterator>
#include <type_traits>

// Provides utility functions for checking iterator categories

//    static_assert(melinda::traits::is_input_iterator_v<
//        std::vector<int>::iterator>);
//
//    static_assert(melinda::traits::is_forward_iterator_v<
//        std::vector<int>::iterator>);
//
//    static_assert(melinda::traits::is_bidirectional_iterator_v<
//        std::vector<int>::iterator>);
//
//    static_assert(melinda::traits::is_random_access_iterator_v<
//        std::vector<int>::iterator>);
//
//    static_assert(malinda::traits::is_input_iterator_v<
//        std::list<int>::iterator>);
//
//    static_assert(melinda::traits::is_forward_access_iterator_v<
//        std::list<int>::iterator>);

namespace melinda::traits
{
    namespace detail
    {
        template<typename Iterator, typename Category>
        struct is_of_category final
            : std::bool_constant<std::is_convertible_v<
                  typename std::iterator_traits<Iterator>::iterator_category,
                  Category>>
        {
            using class_name = is_of_category;
        };
    } // namespace detail

    template<typename Iterator>
    using is_input_iterator =
        detail::is_of_category<Iterator, std::input_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_input_iterator_v =
        is_input_iterator<Iterator>::value;

    template<typename Iterator>
    using is_forward_iterator =
        detail::is_of_category<Iterator, std::forward_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_forward_iterator_v =
        is_forward_iterator<Iterator>::value;

    template<typename Iterator>
    using is_bidirectional_iterator =
        detail::is_of_category<Iterator, std::bidirectional_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_bidirectional_iterator_v =
        is_bidirectional_iterator<Iterator>::value;

    template<typename Iterator>
    using is_random_access_iterator =
        detail::is_of_category<Iterator, std::random_access_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_random_access_iterator_v =
        is_random_access_iterator<Iterator>::value;

    // TODO C++20: contiguous_iterator_tag

    template<typename Iterator>
    using is_output_iterator =
        detail::is_of_category<Iterator, std::output_iterator_tag>;

    template<typename Iterator>
    inline constexpr bool is_output_iterator_v =
        is_output_iterator<Iterator>::value;

} // namespace melinda::traits

#endif
