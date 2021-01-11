#include <catch2/catch.hpp>

#include <algorithm>
#include <array>
#include <cstddef>

#include "ncprot_message.h"

namespace
{
    template<std::size_t N>
    std::array<std::byte, N> generate_sequence()
    {
        auto generator = [i = uint8_t(0)]() mutable noexcept {
            return static_cast<std::byte>(i++);
        };

        std::array<std::byte, N> rv;
        std::generate_n(std::begin(rv), N, generator);

        return rv;
    }
} // namespace

TEST_CASE("Default constructed message is empty")
{
    mel::ncprot::message m;
    REQUIRE(m.size() == 0);
}

TEST_CASE("Message can be constructed from a range of bytes")
{
    std::array<std::byte, 10> bytes = generate_sequence<10>();

    mel::ncprot::message const m =
        mel::ncprot::message(std::cbegin(bytes), std::cend(bytes));

    REQUIRE(m.size() == bytes.size());
    REQUIRE(m.data() != nullptr);
    REQUIRE(std::equal(m.data(), m.data() + m.size(), std::cbegin(bytes)));
}

TEST_CASE("Copying a message preserves the content and does a deep copy")
{
    std::array<std::byte, 10> const bytes = generate_sequence<10>();
    std::array<std::byte, 20> const original_bytes = generate_sequence<20>();

    mel::ncprot::message const m =
        mel::ncprot::message(std::cbegin(bytes), std::cend(bytes));
    mel::ncprot::message const copy_construct = m;

    REQUIRE(m.size() == copy_construct.size());
    REQUIRE(m.data() != copy_construct.data());
    REQUIRE(std::equal(m.data(), m.data() + m.size(), copy_construct.data()));

    mel::ncprot::message copy_assign =
        mel::ncprot::message(std::cbegin(original_bytes),
            std::cend(original_bytes));
    REQUIRE(copy_assign.size() == 20);

    copy_assign = m;
    REQUIRE(m.size() == copy_assign.size());
    REQUIRE(m.data() != copy_assign.data());
    REQUIRE(std::equal(m.data(), m.data() + m.size(), copy_assign.data()));
}
