#ifndef MELINDA_MQLAST_BINARY_STRING_LITERAL
#define MELINDA_MQLAST_BINARY_STRING_LITERAL

#include <concepts>
#include <cstddef>
#include <utility>
#include <vector>

namespace melinda::mqlast
{
    struct [[nodiscard]] binary_string_literal final
    {
    public: // Construction
        template<typename Value>
        constexpr binary_string_literal(Value&& value)
        requires(std::constructible_from<std::vector<std::byte>, Value>)
            : value_{std::forward<Value>(value)}
        {
        }

        constexpr binary_string_literal(binary_string_literal const&) = default;

        constexpr binary_string_literal(
            binary_string_literal&&) noexcept = default;

    public: // Destruction
        constexpr ~binary_string_literal() = default;

    public: // Interface
        [[nodiscard]] constexpr std::vector<std::byte> const& value() const&
        {
            return value_;
        }

        [[nodiscard]] constexpr std::vector<std::byte> value() &&
        {
            return std::move(value_);
        }

    public: // Operators
        constexpr binary_string_literal& operator=(
            binary_string_literal const&) = default;

        constexpr binary_string_literal& operator=(
            binary_string_literal&&) noexcept = default;

        constexpr friend bool operator==(binary_string_literal const&,
            binary_string_literal const&) = default;

    private: // Data
        std::vector<std::byte> value_;
    };
} // namespace melinda::mqlast

#endif
