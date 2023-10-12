#ifndef MELINDA_MQLPRS_AST_IDENTIFIER_INCLUDED
#define MELINDA_MQLPRS_AST_IDENTIFIER_INCLUDED

#include <concepts>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include <mqlprs_ast_delimited_identifier.h>
#include <mqlprs_ast_regular_identifier.h>
#include <mqlprs_ast_unicode_delimited_identifier.h>

namespace melinda::mqlprs::ast
{
    struct [[nodiscard]] identifier final
    {
    public: // Types
        using underlying_type = std::variant<regular_identifier,
            delimited_identifier,
            unicode_delimited_identifier>;

    public: // Construction
        template<typename Identifier>
        constexpr identifier(Identifier&& identifier)
        requires(std::constructible_from<underlying_type, Identifier>)
            : value_{std::forward<Identifier>(identifier)}
        {
        }

        constexpr identifier(identifier const&) = default;

        constexpr identifier(identifier&&) noexcept = default;

    public: // Destruction
        constexpr ~identifier() = default;

    public: // Interface
        template<typename T>
        [[nodiscard]] constexpr bool of_type() const
        {
            return std::holds_alternative<T>(value_);
        }

        [[nodiscard]] constexpr std::string_view contained_type() const
        {
            if (of_type<regular_identifier>())
            {
                return "regular_identifier";
            }
            else if (of_type<delimited_identifier>())
            {
                return "delimited_identifier";
            }
            else
            {
                return "unicode_delimited_identifier";
            }
        }

        [[nodiscard]] constexpr std::string const& body() const&
        {
            if (auto regular = std::get_if<regular_identifier>(&value_))
            {
                return regular->body();
            }
            else if (auto delimited =
                         std::get_if<delimited_identifier>(&value_))
            {
                return delimited->body();
            }
            else
            {
                return std::get<unicode_delimited_identifier>(value_).body();
            }
        }

        [[nodiscard]] constexpr std::string body() &&
        {
            if (auto regular = std::get_if<regular_identifier>(&value_))
            {
                return std::move(*regular).body();
            }
            else if (auto delimited =
                         std::get_if<delimited_identifier>(&value_))
            {
                return std::move(*delimited).body();
            }
            else
            {
                return std::get<unicode_delimited_identifier>(std::move(value_))
                    .body();
            }
        }

    public: // Operators
        constexpr identifier& operator=(identifier const&) = default;

        constexpr identifier& operator=(identifier&&) noexcept = default;

        constexpr friend bool operator==(identifier const&,
            identifier const&) = default;

    private: // Data
        underlying_type value_;
    };

    struct [[nodiscard]] multipart_identifier final
    {
    public: // Types
        using underlying_type = std::vector<identifier>;

    public: // Construction
        template<typename Identifiers>
        constexpr multipart_identifier(Identifiers&& parts)
        requires(std::constructible_from<underlying_type, Identifiers>)
            : parts_{std::forward<Identifiers>(parts)}
        {
        }

        constexpr multipart_identifier(multipart_identifier const&) = default;

        constexpr multipart_identifier(
            multipart_identifier&&) noexcept = default;

    public: // Destruction
        constexpr ~multipart_identifier() = default;

    public: // Interface
        [[nodiscard]] constexpr std::vector<identifier> const& parts() const&
        {
            return parts_;
        }

        [[nodiscard]] constexpr std::vector<identifier> parts() &&
        {
            return std::move(parts_);
        }

    public: // Operators
        constexpr multipart_identifier& operator=(
            multipart_identifier const&) = default;

        constexpr multipart_identifier& operator=(
            multipart_identifier&&) noexcept = default;

        friend bool operator==(multipart_identifier const&,
            multipart_identifier const&) = default;

    private: // Data
        underlying_type parts_;
    };
} // namespace melinda::mqlprs::ast

#endif
