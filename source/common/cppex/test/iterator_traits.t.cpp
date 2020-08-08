#include "../include/iterator_traits.h"
#include "catch2/catch.hpp"

#include <forward_list>
#include <iterator>
#include <list>
#include <vector>

namespace
{
    using input_iterator = std::istream_iterator<char>;
    using forward_iterator = std::forward_list<char>::const_iterator;
    using bidirectional_iterator = std::list<char>::const_iterator;
    using random_access_iterator = std::vector<char>::const_iterator;
    using output_iterator = std::ostream_iterator<char>;

    template<typename T, typename Enable = void>
    class class_sfinae final
    {
    public: // Constants
        static constexpr uint8_t value = 1;

    public: // Types
        using class_name = class_sfinae;
    };

    template<typename T>
    class class_sfinae<T,
        std::enable_if_t<mel::cppex::is_random_access_iterator_v<T>>>
        final
    {
    public: // Constants
        static constexpr uint8_t value = 2;

    public: // Types
        using class_name = class_sfinae;
    };

    template<typename T>
    class class_sfinae<T, std::enable_if_t<mel::cppex::is_output_iterator_v<T>>>
        final
    {
    public: // Constants
        static constexpr uint8_t value = 3;

    public: // Types
        using class_name = class_sfinae;
    };

    template<typename T,
        std::enable_if_t<mel::cppex::is_input_iterator_v<T>, int> = 0>
    [[nodiscard]] constexpr uint8_t method_sfinae([[maybe_unused]] T&& it)
    {
        return uint8_t(1);
    }

    template<typename T,
        std::enable_if_t<mel::cppex::is_random_access_iterator_v<T>, int> = 0>
    [[nodiscard]] constexpr uint8_t method_sfinae([[maybe_unused]] T&& it)
    {
        return uint8_t(2);
    }
} // namespace

TEST_CASE("Input iterator tests")
{
    using namespace mel::cppex;

    static_assert(atleast_input_iterator_v<input_iterator>);
    static_assert(is_input_iterator_v<input_iterator>);

    static_assert(!atleast_forward_iterator_v<input_iterator>);
    static_assert(!is_forward_iterator_v<input_iterator>);

    static_assert(!atleast_bidirectional_iterator_v<input_iterator>);
    static_assert(!is_bidirectional_iterator_v<input_iterator>);

    static_assert(!atleast_random_access_iterator_v<input_iterator>);
    static_assert(!is_random_access_iterator_v<input_iterator>);

    static_assert(!atleast_output_iterator_v<input_iterator>);
    static_assert(!is_output_iterator_v<input_iterator>);
}

TEST_CASE("Forward iterator tests")
{
    using namespace mel::cppex;

    static_assert(atleast_input_iterator_v<forward_iterator>);
    static_assert(!is_input_iterator_v<forward_iterator>);

    static_assert(atleast_forward_iterator_v<forward_iterator>);
    static_assert(is_forward_iterator_v<forward_iterator>);

    static_assert(!atleast_bidirectional_iterator_v<forward_iterator>);
    static_assert(!is_bidirectional_iterator_v<forward_iterator>);

    static_assert(!atleast_random_access_iterator_v<forward_iterator>);
    static_assert(!is_random_access_iterator_v<forward_iterator>);

    static_assert(!atleast_output_iterator_v<forward_iterator>);
    static_assert(!is_output_iterator_v<forward_iterator>);
}

TEST_CASE("Bidirectional iterator tests")
{
    using namespace mel::cppex;

    static_assert(atleast_input_iterator_v<bidirectional_iterator>);
    static_assert(!is_input_iterator_v<bidirectional_iterator>);

    static_assert(atleast_forward_iterator_v<bidirectional_iterator>);
    static_assert(!is_forward_iterator_v<bidirectional_iterator>);

    static_assert(atleast_bidirectional_iterator_v<bidirectional_iterator>);
    static_assert(is_bidirectional_iterator_v<bidirectional_iterator>);

    static_assert(!atleast_random_access_iterator_v<bidirectional_iterator>);
    static_assert(!is_random_access_iterator_v<bidirectional_iterator>);

    static_assert(!atleast_output_iterator_v<bidirectional_iterator>);
    static_assert(!is_output_iterator_v<bidirectional_iterator>);
}

TEST_CASE("Random access iterator tests")
{
    using namespace mel::cppex;

    static_assert(atleast_input_iterator_v<random_access_iterator>);
    static_assert(!is_input_iterator_v<random_access_iterator>);

    static_assert(atleast_forward_iterator_v<random_access_iterator>);
    static_assert(!is_forward_iterator_v<random_access_iterator>);

    static_assert(atleast_bidirectional_iterator_v<random_access_iterator>);
    static_assert(!is_bidirectional_iterator_v<random_access_iterator>);

    static_assert(atleast_random_access_iterator_v<random_access_iterator>);
    static_assert(is_random_access_iterator_v<random_access_iterator>);

    static_assert(!atleast_output_iterator_v<random_access_iterator>);
    static_assert(!is_output_iterator_v<random_access_iterator>);
}

TEST_CASE("Output iterator tests")
{
    using namespace mel::cppex;

    static_assert(!atleast_input_iterator_v<output_iterator>);
    static_assert(!is_input_iterator_v<output_iterator>);

    static_assert(!atleast_forward_iterator_v<output_iterator>);
    static_assert(!is_forward_iterator_v<output_iterator>);

    static_assert(!atleast_bidirectional_iterator_v<output_iterator>);
    static_assert(!is_bidirectional_iterator_v<output_iterator>);

    static_assert(!atleast_random_access_iterator_v<output_iterator>);
    static_assert(!is_random_access_iterator_v<output_iterator>);

    static_assert(atleast_output_iterator_v<output_iterator>);
    static_assert(is_output_iterator_v<output_iterator>);
}

TEST_CASE("Iterator traits can be used for class SFINAE")
{
    static_assert(class_sfinae<forward_iterator>::value == uint8_t(1));
    static_assert(class_sfinae<random_access_iterator>::value == uint8_t(2));
    static_assert(class_sfinae<output_iterator>::value == uint8_t(3));
}

TEST_CASE("Iterator traits can be used for method SFINAE")
{
    static_assert(method_sfinae(input_iterator()) == uint8_t(1));
    static_assert(method_sfinae(random_access_iterator()) == uint8_t(2));
}
