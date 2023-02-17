#ifndef MELINDA_MBLCXX_RESULT_INCLUDED
#define MELINDA_MBLCXX_RESULT_INCLUDED

#include <concepts>
#include <functional>
#include <system_error>
#include <type_traits>
#include <utility>
#include <variant>

#include <mblcxx_always_false.h>
#include <mblcxx_sops.h>

namespace melinda::mblcxx::detail
{
    template<typename T1, typename T2>
    using either = std::variant<T1, T2>;

    template<typename Callable, typename... Args>
    concept nothrow_invocable = std::invocable<Callable, Args...> &&
        noexcept(
            std::invoke(std::declval<Callable>(), std::declval<Args>()...));

    template<typename T>
    concept move_assignable = std::is_move_assignable_v<T>;

} // namespace melinda::mblcxx::detail

namespace melinda::mblcxx
{
    template<typename T, typename E = std::error_code>
    class result;

    template<typename T, typename E = std::error_code>
    struct res final : sops::none
    {
    public:
        template<typename... Args>
        static result<T, E> ok(Args&&... args) noexcept(noexcept(
            result<T, E>(std::in_place_index<0>, std::forward<Args>(args)...)));

        template<typename... Args>
        static result<T, E> error(Args&&... args) noexcept(noexcept(
            result<T, E>(std::in_place_index<1>, std::forward<Args>(args)...)));
    };

    template<typename T, typename E>
    class [[nodiscard]] result final
    {
    public: // Types
        using ok_type = T;
        using error_type = E;

    private: // Types
        using either_t = detail::either<T, E>;

    public: // Constants
        static constexpr std::size_t ok_index = 0;
        static constexpr std::size_t error_index = 1;

    public: // Construction
        result() = default;

        template<typename U, typename... Args>
        result(std::in_place_type_t<U> alternative, Args&&... value) noexcept(
            std::is_nothrow_constructible_v<T, Args...>)
        requires(std::constructible_from<T, Args...> &&
            (!std::same_as<T, E> &&
                (std::same_as<U, T> || std::same_as<U, E>) ))
            : value_(alternative, std::forward<Args>(value)...)
        {
        }

        template<std::size_t I, typename... Args>
        result(std::in_place_index_t<I> index, Args&&... value) noexcept(
            std::is_nothrow_constructible_v<
                std::variant_alternative_t<I, either_t>,
                Args...>)
        requires(
            std::constructible_from<std::variant_alternative_t<I, either_t>,
                Args...> &&
            (std::same_as<std::variant_alternative_t<I, either_t>, T> ||
                std::same_as<std::variant_alternative_t<I, either_t>, E>) )
            : value_(index, std::forward<Args>(value)...)
        {
        }

        result(result const& other) = default;

        result(result&& other) noexcept
        requires(std::move_constructible<T>)
        = default;

    public: // Interface
        template<typename OnSuccess>
        result<std::invoke_result_t<OnSuccess, T>, E> map(
            OnSuccess on_success) const
        requires(std::invocable<OnSuccess, T>)
        {
            using success_t = std::invoke_result_t<OnSuccess, T>;

            if (*this)
            {
                return res<success_t, E>::ok(on_success(ok()));
            }
            return res<success_t, E>::error(error());
        }

        template<typename OnSuccess, typename DefaultT>
        std::common_type_t<std::invoke_result_t<OnSuccess, T>, DefaultT>
        map_or(OnSuccess on_success, DefaultT&& default_value) const
        requires(std::invocable<OnSuccess, T>)
        {
            using rv_t = std::common_type_t<std::invoke_result_t<OnSuccess, T>,
                DefaultT>;

            if (*this)
            {
                return rv_t(on_success(ok()));
            }
            return rv_t(default_value);
        }

        template<typename OnSuccess, typename OnError>
        std::common_type_t<std::invoke_result_t<OnSuccess, T>,
            std::invoke_result_t<OnError, E>>
        map_or_else(OnSuccess on_success, OnError on_error) const
        requires(std::invocable<OnSuccess, T> && std::invocable<OnError, E>)
        {
            using rv_t = std::common_type_t<std::invoke_result_t<OnSuccess, T>,
                std::invoke_result_t<OnError, E>>;

            if (*this)
            {
                return rv_t(on_success(ok()));
            }
            return rv_t(on_error(error()));
        }

        T& ok() { return std::get<ok_index>(value_); }

        T const& ok() const { return std::get<ok_index>(value_); }

        E& error() { return std::get<error_index>(value_); }

        E const& error() const { return std::get<error_index>(value_); }

    public: // Operators
        result& operator=(result const& other) = default;

        result& operator=(result&& other) noexcept
        requires(detail::move_assignable<T>)
        = default;

    public: // Conversions
        operator bool() const noexcept { return value_.index() == ok_index; }

    public: // Destruction
        ~result() noexcept = default;

    private: // Data
        either_t value_;
    };

    template<typename T, typename E>
    template<typename... Args>
    result<T, E> res<T, E>::ok(Args&&... args) noexcept(noexcept(
        result<T, E>(std::in_place_index<0>, std::forward<Args>(args)...)))
    {
        return result<T, E>(std::in_place_index<0>,
            std::forward<Args>(args)...);
    }

    template<typename T, typename E>
    template<typename... Args>
    result<T, E> res<T, E>::error(Args&&... args) noexcept(noexcept(
        result<T, E>(std::in_place_index<1>, std::forward<Args>(args)...)))
    {
        return result<T, E>(std::in_place_index<1>,
            std::forward<Args>(args)...);
    }

} // namespace melinda::mblcxx

#endif
