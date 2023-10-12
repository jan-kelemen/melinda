#ifndef MELINDA_MQLPRS_AST_UNICODE_DELIMITED_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_AST_UNICODE_DELIMITED_IDENTIFIER_INCLUDED

#include <concepts>
#include <string>
#include <utility>

namespace melinda::mqlprs::ast
{
    struct [[nodiscard]] unicode_delimited_identifier final
    {
    public: // Construction
        template<typename Body>
        constexpr unicode_delimited_identifier(Body&& body,
            char escape_character = '\\')
        requires(std::constructible_from<std::string, Body>)
            : body_{std::forward<Body>(body)}
            , escape_character_{escape_character}
        {
        }

        constexpr unicode_delimited_identifier(
            unicode_delimited_identifier const&) = default;

        constexpr unicode_delimited_identifier(
            unicode_delimited_identifier&&) noexcept = default;

    public: // Destruction
        constexpr ~unicode_delimited_identifier() = default;

    public: // Interface
        [[nodiscard]] constexpr std::string const& body() const&
        {
            return body_;
        }

        [[nodiscard]] constexpr std::string body() &&
        {
            return std::move(body_);
        }

        [[nodiscard]] constexpr char escape_character() const
        {
            return escape_character_;
        }

    public: // Operators
        constexpr unicode_delimited_identifier& operator=(
            unicode_delimited_identifier const&) = default;

        constexpr unicode_delimited_identifier& operator=(
            unicode_delimited_identifier&&) noexcept = default;

        constexpr friend bool operator==(unicode_delimited_identifier const&,
            unicode_delimited_identifier const&) = default;

    private: // Data
        std::string body_;
        char escape_character_;
    };

} // namespace melinda::mqlprs::ast

#endif
