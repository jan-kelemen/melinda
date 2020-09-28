#ifndef MELINDA_COMMON_CPPEX_RESULT_INCLUDED
#define MELINDA_COMMON_CPPEX_RESULT_INCLUDED

#include <concepts>
#include <system_error>
#include <type_traits>
#include <variant>

namespace mel::cppex
{
    template<typename T>
    concept is_result_value =
        !std::disjunction_v<std::is_convertible<T, std::error_code>,
            std::is_convertible<T, std::error_condition>>;

    template<typename T>
    requires(is_result_value<T>) class result final
    {
    public: // Construction
        template<typename... Args>
        result(Args&&... value) requires(std::constructible_from<T, Args...>)
            : value_(std::forward<Args>(value)...)
        {
        }

        explicit result(std::error_code error) noexcept { }

        result(result const& other) = default;

        result(result&& other) noexcept = default;

    public: // Operators
        result& operator=(result const& other);

        result& operator=(result&& other) noexcept;

        explicit operator bool() noexcept { return value_.index() == 0; }

    public: // Destruction
        ~result() noexcept = default;

    public: // Data
        std::variant<T, std::error_code> value_;
    };
} // namespace mel::cppex

#endif
