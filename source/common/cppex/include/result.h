#ifndef MELINDA_COMMON_CPPEX_RESULT_INCLUDED
#define MELINDA_COMMON_CPPEX_RESULT_INCLUDED

#include <concepts>
#include <functional>
#include <system_error>
#include <type_traits>
#include <variant>

#include "always_false.h"

namespace mel::cppex
{
    namespace detail
    {
        template<typename T1, typename T2>
        using either = std::variant<T1, T2>;

        template<typename Callable, typename... Args>
        concept
            nothrow_invocable = std::invocable<Callable, Args...>&& noexcept(
                std::invoke(std::declval<Callable>(), std::declval<Args>()...));

        template<typename T>
        concept move_assignable = std::is_move_assignable_v<T>;

        template<typename T>
        concept result_value = !std::convertible_to<T, std::error_code> &&
            !std::convertible_to<T, std::error_condition>;

        template<typename Callable, typename T>
        concept success_callable = std::invocable<Callable, T>;

        template<typename Callable>
        concept error_callable = std::invocable<Callable, std::error_code>;
    } // namespace detail

    // TODO-JK: result shouldn't be constrained to one type only, error should
    // also be parametrizable
    template<typename T>
    requires(detail::result_value<T>) class [[nodiscard]] result final
    {
    private: // Types
        using either_t = detail::either<T, std::error_code>;

    public: // Construction
        result() = default;

        template<typename... Args>
        result(Args&&... value) noexcept(
            std::is_nothrow_constructible_v<T, Args...>)
            requires(std::constructible_from<T, Args...>)
            : value_(T(std::forward<Args>(value)...))
        {
        }

        explicit result(std::error_code error) noexcept(
            std::is_nothrow_constructible_v<either_t,
                std::add_rvalue_reference_t<std::error_code>>)
            : value_(std::move(error))
        {
        }

        result(result const& other) = default;

        result(result&& other) noexcept
            requires(std::move_constructible<T>) = default;

    public: // Interface
        template<typename OnSuccess>
        result<std::invoke_result_t<OnSuccess, T>> map(
            OnSuccess on_success) const
            requires(detail::success_callable<OnSuccess, T>)
        {
            using rv_t = result<std::invoke_result_t<OnSuccess, T>>;

            return std::visit(
                [&on_success](auto&& arg) -> rv_t {
                    using U = std::decay_t<decltype(arg)>;
                    if constexpr (std::same_as<U, T>)
                    {
                        return rv_t(on_success(arg));
                    }
                    else if constexpr (std::same_as<U, std::error_code>)
                    {
                        return rv_t(arg);
                    }
                    else
                    {
                        static_assert(always_false_v<U>,
                            "non-exhaustive visitor!");
                    }
                },
                value_);
        }

        template<typename OnSuccess, typename DefaultT>
        std::common_type_t<std::invoke_result_t<OnSuccess, T>, DefaultT>
        map_or(OnSuccess on_success, DefaultT&& default_value) const
            requires(detail::success_callable<OnSuccess, T>)
        {
            using rv_t = std::common_type_t<std::invoke_result_t<OnSuccess, T>,
                DefaultT>;

            // TODO-JK: CppCon passing values back and forth
            return std::visit(
                [&on_success,
                    default_value = std::forward<DefaultT>(default_value)](
                    auto&& arg) -> rv_t {
                    using U = std::decay_t<decltype(arg)>;
                    if constexpr (std::same_as<U, T>)
                    {
                        return rv_t(on_success(arg));
                    }
                    else if constexpr (std::same_as<U, std::error_code>)
                    {
                        return rv_t(default_value);
                    }
                    else
                    {
                        static_assert(!always_false_v<U>,
                            "non-exhaustive visitor!");
                    }
                },
                value_);
        }

        template<typename OnSuccess, typename OnError>
        std::common_type_t<std::invoke_result_t<OnSuccess, T>,
            std::invoke_result_t<OnError, std::error_code>>
        map_or_else(OnSuccess on_success, OnError on_error) const
            requires(detail::success_callable<OnSuccess, T>&&
                    detail::error_callable<OnError>)
        {
            using rv_t = std::common_type_t<std::invoke_result_t<OnSuccess, T>,
                std::invoke_result_t<OnError, std::error_code>>;

            // https://en.cppreference.com/w/cpp/utility/variant/visit
            return std::visit(
                [&on_success, &on_error](auto&& arg) -> rv_t {
                    using U = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<U, T>)
                    {
                        return rv_t(on_success(arg));
                    }
                    else if constexpr (std::is_same_v<U, std::error_code>)
                    {
                        return rv_t(on_error(arg));
                    }
                    else
                    {
                        static_assert(!always_false_v<U>,
                            "non-exhaustive visitor!");
                    }
                },
                value_);
        }

    public: // Operators
        result& operator=(result const& other) = default;

        result& operator=(result&& other) noexcept
            requires(detail::move_assignable<T>) = default;

    public: // Conversions
        operator bool() const noexcept { return value_.index() == 0; }

        explicit operator T&() { return std::get<T>(value_); }

        explicit operator T const &() const { return std::get<T>(value_); }

        explicit operator std::error_code() const
        {
            return std::get<std::error_code>(value_);
        }

    public: // Destruction
        ~result() noexcept = default;

    private: // Data
        either_t value_;
    };

} // namespace mel::cppex

#endif
