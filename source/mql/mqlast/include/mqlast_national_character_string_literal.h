#ifndef MELINDA_MQLAST_NATIONAL_CHARACTER_STRING_LITERAL
#define MELINDA_MQLAST_NATIONAL_CHARACTER_STRING_LITERAL

#include <concepts>
#include <string>
#include <utility>

#include <mqlast_identifier.h>

namespace melinda::mqlast
{
    struct [[nodiscard]] national_character_string_literal final
    {
    public: // Construction
        template<typename Value>
        constexpr national_character_string_literal(Value&& value)
        requires(std::constructible_from<std::string, Value>)
            : value_{std::forward<Value>(value)}
        {
        }

        constexpr national_character_string_literal(
            national_character_string_literal const&) = default;

        constexpr national_character_string_literal(
            national_character_string_literal&&) noexcept = default;

    public: // Destruction
        constexpr ~national_character_string_literal() = default;

    public: // Interface
        [[nodiscard]] constexpr std::string const& value() const&
        {
            return value_;
        }

        [[nodiscard]] constexpr std::string value() &&
        {
            return std::move(value_);
        }

    public: // Operators
        constexpr national_character_string_literal& operator=(
            national_character_string_literal const&) = default;

        constexpr national_character_string_literal& operator=(
            national_character_string_literal&&) noexcept = default;

        constexpr friend bool operator==(
            national_character_string_literal const&,
            national_character_string_literal const&) = default;

    private: // Data
        std::string value_;
    };
} // namespace melinda::mqlast

#endif
