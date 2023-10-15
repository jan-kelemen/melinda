#ifndef MELINDA_MQLAST_NUMERIC_LITERAL_INCLUDED
#define MELINDA_MQLAST_NUMERIC_LITERAL_INCLUDED

#include <concepts>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

#include <mqlast_common.h>

namespace melinda::mqlast
{
    struct [[nodiscard]] exact_numeric_literal final
    {
    public: // Types
        using underlying_type = std::optional<std::string>;

    public: // Construction
        template<typename Whole, typename Decimal = underlying_type>
        constexpr exact_numeric_literal(Whole&& whole,
            Decimal&& decimal = std::nullopt)
        requires(std::constructible_from<underlying_type, Whole> &&
                    std::constructible_from<underlying_type, Decimal>)
            : whole_{std::forward<Whole>(whole)}
            , decimal_{std::forward<Decimal>(decimal)}
        {
        }

        constexpr exact_numeric_literal(exact_numeric_literal const&) = default;

        constexpr exact_numeric_literal(
            exact_numeric_literal&&) noexcept = default;

    public: // Destruction
        constexpr ~exact_numeric_literal() = default;

    public: // Interface
        [[nodiscard]] constexpr std::optional<std::string> const& whole() const&
        {
            return whole_;
        }

        [[nodiscard]] constexpr std::optional<std::string> whole() &&
        {
            return std::move(whole_);
        }

        [[nodiscard]] constexpr std::optional<std::string> const&
        decimal() const&
        {
            return decimal_;
        }

        [[nodiscard]] constexpr std::optional<std::string> decimal() &&
        {
            return std::move(decimal_);
        }

    public: // Operators
        constexpr exact_numeric_literal& operator=(
            exact_numeric_literal const&) = default;

        constexpr exact_numeric_literal& operator=(
            exact_numeric_literal&&) noexcept = default;

        constexpr friend bool operator==(exact_numeric_literal const&,
            exact_numeric_literal const&) = default;

    private: // Data
        std::optional<std::string> whole_;
        std::optional<std::string> decimal_;
    };

    struct [[nodiscard]] approximate_numeric_literal final
    {
    public: // Construction
        template<typename Mantissa, typename Sign, typename Exponent>
        constexpr approximate_numeric_literal(Mantissa&& mantissa,
            Sign&& s,
            Exponent&& exponent)
        requires(std::constructible_from<exact_numeric_literal, Mantissa> &&
                    std::constructible_from<std::optional<enum sign>, Sign> &&
                    std::constructible_from<std::string, Exponent>)
            : mantissa_{std::forward<Mantissa>(mantissa)}
            , sign_{std::forward<Sign>(s)}
            , exponent_{std::forward<Exponent>(exponent)}
        {
        }

        constexpr approximate_numeric_literal(
            approximate_numeric_literal const&) = default;

        constexpr approximate_numeric_literal(
            approximate_numeric_literal&&) noexcept = default;

    public: // Interface
        [[nodiscard]] exact_numeric_literal const& mantissa() const&
        {
            return mantissa_;
        }

        [[nodiscard]] exact_numeric_literal mantissa() &&
        {
            return std::move(mantissa_);
        }

        [[nodiscard]] std::optional<sign> sign() const { return sign_; }

        [[nodiscard]] std::string const& exponent() const& { return exponent_; }

        [[nodiscard]] std::string exponent() && { return exponent_; }

    public: // Operators
        constexpr approximate_numeric_literal& operator=(
            approximate_numeric_literal const&) = default;

        constexpr approximate_numeric_literal& operator=(
            approximate_numeric_literal&&) noexcept = default;

        constexpr friend bool operator==(approximate_numeric_literal const&,
            approximate_numeric_literal const&) = default;

    public: // Destruction
        constexpr ~approximate_numeric_literal() = default;

    private: // Data
        exact_numeric_literal mantissa_;
        std::optional<enum sign> sign_;
        std::string exponent_;
    };

    template<bool Signed>
    struct [[nodiscard]] numeric_literal final : public sign_mixin<Signed>
    {
    public: // Types
        using underlying_type =
            std::variant<exact_numeric_literal, approximate_numeric_literal>;

    public: // Construction
        template<typename Sign, typename Literal>
        constexpr numeric_literal(Sign&& sign, Literal&& literal)
        requires(std::constructible_from<sign_mixin<Signed>, Sign> &&
                    std::constructible_from<underlying_type, Literal>)
            : sign_mixin<Signed>{std::forward<Sign>(sign)}
            , literal_{std::forward<Literal>(literal)}
        {
        }

        template<typename Literal>
        constexpr numeric_literal(Literal&& literal)
        requires(std::constructible_from<underlying_type, Literal>)
            : literal_{std::forward<Literal>(literal)}
        {
        }

        constexpr numeric_literal(numeric_literal const&) = default;

        constexpr numeric_literal(numeric_literal&&) noexcept = default;

    public: // Destruction
        constexpr ~numeric_literal() = default;

    public: // Interface
        template<typename T>
        [[nodiscard]] constexpr bool of_type() const
        {
            return std::holds_alternative<T>(literal_);
        }

        [[nodiscard]] constexpr std::string_view contained_type() const
        {
            if (of_type<exact_numeric_literal>())
            {
                return "exact_numeric_literal";
            }
            else
            {
                return "approximate_numeric_literal";
            }
        }

        [[nodiscard]] constexpr underlying_type const& literal() const&
        {
            return literal_;
        }

        [[nodiscard]] constexpr underlying_type literal() &&
        {
            return std::move(literal_);
        }

    public: // Operators
        constexpr numeric_literal& operator=(numeric_literal const&) = default;

        constexpr numeric_literal& operator=(numeric_literal&&) = default;

        constexpr friend bool operator==(numeric_literal const&,
            numeric_literal const&) = default;

    private: // Data
        underlying_type literal_;
    };

    using unsigned_numeric_literal = numeric_literal<false>;

    using signed_numeric_literal = numeric_literal<true>;
} // namespace melinda::mqlast
#endif
