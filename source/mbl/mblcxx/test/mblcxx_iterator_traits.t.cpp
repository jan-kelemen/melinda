#include <forward_list>
#include <iterator>
#include <list>
#include <vector>

#include <catch2/catch.hpp>

#include <mblcxx_iterator_traits.h>

using namespace melinda;

namespace
{
    using input_iterator = std::istream_iterator<char>;
    using forward_iterator = std::forward_list<char>::const_iterator;
    using bidirectional_iterator = std::list<char>::const_iterator;
    using random_access_iterator = std::vector<char>::const_iterator;
    using output_iterator = std::ostream_iterator<char>;
} // namespace

TEST_CASE("Input iterator tests")
{
    static_assert(mblcxx::is_input_iterator_v<input_iterator>);
    static_assert(!mblcxx::is_forward_iterator_v<input_iterator>);
    static_assert(!mblcxx::is_bidirectional_iterator_v<input_iterator>);
    static_assert(!mblcxx::is_random_access_iterator_v<input_iterator>);
    static_assert(!mblcxx::is_output_iterator_v<input_iterator>);
}

TEST_CASE("Forward iterator tests")
{
    static_assert(mblcxx::is_input_iterator_v<forward_iterator>);
    static_assert(mblcxx::is_forward_iterator_v<forward_iterator>);
    static_assert(!mblcxx::is_bidirectional_iterator_v<forward_iterator>);
    static_assert(!mblcxx::is_random_access_iterator_v<forward_iterator>);
    static_assert(!mblcxx::is_output_iterator_v<forward_iterator>);
}

TEST_CASE("Bidirectional iterator tests")
{
    static_assert(mblcxx::is_input_iterator_v<bidirectional_iterator>);
    static_assert(mblcxx::is_forward_iterator_v<bidirectional_iterator>);
    static_assert(mblcxx::is_bidirectional_iterator_v<bidirectional_iterator>);
    static_assert(!mblcxx::is_random_access_iterator_v<bidirectional_iterator>);
    static_assert(!mblcxx::is_output_iterator_v<bidirectional_iterator>);
}

TEST_CASE("Random access iterator tests")
{
    static_assert(mblcxx::is_input_iterator_v<random_access_iterator>);
    static_assert(mblcxx::is_forward_iterator_v<random_access_iterator>);
    static_assert(mblcxx::is_bidirectional_iterator_v<random_access_iterator>);
    static_assert(mblcxx::is_random_access_iterator_v<random_access_iterator>);
    static_assert(!mblcxx::is_output_iterator_v<random_access_iterator>);
}

TEST_CASE("Output iterator tests")
{
    static_assert(!mblcxx::is_input_iterator_v<output_iterator>);
    static_assert(!mblcxx::is_forward_iterator_v<output_iterator>);
    static_assert(!mblcxx::is_bidirectional_iterator_v<output_iterator>);
    static_assert(!mblcxx::is_random_access_iterator_v<output_iterator>);
    static_assert(mblcxx::is_output_iterator_v<output_iterator>);
}
