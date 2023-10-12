#ifndef MELINDA_MQLPRS_AST_DELIMITED_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_AST_DELIMITED_IDENTIFIER_INCLUDED

#include <concepts>
#include <string>
#include <utility>

namespace melinda::mqlprs::ast
{
    struct [[nodiscard]] delimited_identifier final
    {
    public: // Construction
        template<typename Body>
        constexpr delimited_identifier(Body&& body)
        requires(std::constructible_from<std::string, Body>)
            : body_{std::forward<Body>(body)}
        {
        }

        constexpr delimited_identifier(delimited_identifier const&) = default;

        constexpr delimited_identifier(
            delimited_identifier&&) noexcept = default;

    public: // Destruction
        constexpr ~delimited_identifier() = default;

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
        constexpr delimited_identifier& operator=(
            delimited_identifier const&) = default;

        constexpr delimited_identifier& operator=(
            delimited_identifier&&) noexcept = default;

        constexpr friend bool operator==(delimited_identifier const&,
            delimited_identifier const&) = default;

    private: // Data
        std::string body_;
    };

} // namespace melinda::mqlprs::ast

#endif
