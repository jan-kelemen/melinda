#ifndef MELINDA_MQLPRS_AST_REGULAR_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_AST_REGULAR_IDENTIFIER_INCLUDED

#include <concepts>
#include <string>
#include <utility>

namespace melinda::mqlprs::ast
{
    struct [[nodiscard]] regular_identifier final
    {
    public: // Construction
        template<typename Body>
        constexpr regular_identifier(Body&& body)
        requires(std::constructible_from<std::string, Body>)
            : body_{std::forward<Body>(body)}
        {
        }

        constexpr regular_identifier(regular_identifier const&) = default;

        constexpr regular_identifier(regular_identifier&&) noexcept = default;

    public: // Destruction
        constexpr ~regular_identifier() = default;

    public: // Interface
        [[nodiscard]] constexpr std::string const& body() const&
        {
            return body_;
        }

        [[nodiscard]] constexpr std::string body() &&
        {
            return std::move(body_);
        }

    public: // Operators
        constexpr regular_identifier& operator=(
            regular_identifier const&) = default;

        constexpr regular_identifier& operator=(
            regular_identifier&&) noexcept = default;

        constexpr friend bool operator==(regular_identifier const&,
            regular_identifier const&) = default;

    private: // Data
        std::string body_;
    };

} // namespace melinda::mqlprs::ast

#endif
