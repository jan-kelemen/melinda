#ifndef MELINDA_MQLAST_COMMON_INCLUDED
#define MELINDA_MQLAST_COMMON_INCLUDED

#include <concepts>
#include <optional>
#include <utility>

namespace melinda::mqlast
{
    enum class [[nodiscard]] sign : char
    {
        plus = '+',
        minus = '-',
    };

    template<bool Signed>
    struct [[nodiscard]] sign_mixin
    {
        constexpr friend bool operator==(sign_mixin const&,
            sign_mixin const&) = default;
    };

    template<>
    struct [[nodiscard]] sign_mixin<true>
    {
    public: // Construction
        constexpr sign_mixin() = default;

        template<typename Sign>
        constexpr sign_mixin(Sign&& sign)
        requires(std::constructible_from<std::optional<enum sign>, Sign>)
            : sign_{std::forward<Sign>(sign)}
        {
        }

    public: // Destruction
        constexpr ~sign_mixin() = default;

    public: // Interface
        [[nodiscard]] constexpr std::optional<sign> sign() const
        {
            return sign_;
        }

    public: // Operators
        constexpr friend bool operator==(sign_mixin const&,
            sign_mixin const&) = default;

    protected: // Data
        std::optional<enum sign> sign_;
    };
} // namespace melinda::mqlast
#endif
