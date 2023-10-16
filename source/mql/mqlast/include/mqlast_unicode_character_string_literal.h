#ifndef MELINDA_MQLAST_UNICODE_CHARACTER_STRING_LITERAL
#define MELINDA_MQLAST_UNICODE_CHARACTER_STRING_LITERAL

#include <concepts>
#include <optional>
#include <string>
#include <utility>

#include <mqlast_identifier.h>

namespace melinda::mqlast
{
    struct [[nodiscard]] unicode_character_string_literal final
    {
    public: // Construction
        template<typename CharacterSet, typename Value>
        constexpr unicode_character_string_literal(CharacterSet&& character_set,
            Value&& value,
            char escape_character = '\\')
        requires(std::constructible_from<std::optional<multipart_identifier>,
                     CharacterSet> &&
                    std::constructible_from<std::string, Value>)
            : character_set_{std::forward<CharacterSet>(character_set)}
            , value_{std::forward<Value>(value)}
            , escape_character_{escape_character}
        {
        }

        constexpr unicode_character_string_literal(
            unicode_character_string_literal const&) = default;

        constexpr unicode_character_string_literal(
            unicode_character_string_literal&&) noexcept = default;

    public: // Destruction
        constexpr ~unicode_character_string_literal() = default;

    public: // Interface
        [[nodiscard]] constexpr std::optional<multipart_identifier> const&
        character_set() const&
        {
            return character_set_;
        }

        [[nodiscard]] constexpr std::optional<multipart_identifier>
        character_set() &&
        {
            return std::move(character_set_);
        }

        [[nodiscard]] constexpr std::string const& value() const&
        {
            return value_;
        }

        [[nodiscard]] constexpr std::string value() &&
        {
            return std::move(value_);
        }

        [[nodiscard]] char escape_character() const
        {
            return escape_character_;
        }

    public: // Operators
        constexpr unicode_character_string_literal& operator=(
            unicode_character_string_literal const&) = default;

        constexpr unicode_character_string_literal& operator=(
            unicode_character_string_literal&&) noexcept = default;

        constexpr friend bool operator==(
            unicode_character_string_literal const&,
            unicode_character_string_literal const&) = default;

    private: // Data
        std::optional<multipart_identifier> character_set_;
        std::string value_;
        char escape_character_;
    };
} // namespace melinda::mqlast

#endif
