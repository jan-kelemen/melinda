#ifndef MELINDA_MQLAST_DATETIME_LITERAL_INCLUDED
#define MELINDA_MQLAST_DATETIME_LITERAL_INCLUDED

#include <concepts>
#include <cstdint>
#include <optional>
#include <string_view>
#include <utility>
#include <variant>

#include <mqlast_common.h>

namespace melinda::mqlast
{
    struct [[nodiscard]] date_literal final
    {
    public: // Construction
        template<typename Day, typename Month, typename Year>
        constexpr date_literal(Day&& day, Month&& month, Year&& year)
        requires(std::constructible_from<uint8_t, Day> &&
                    std::constructible_from<uint8_t, Month> &&
                    std::constructible_from<int64_t, Year>)
            : day_{std::forward<Day>(day)}
            , month_{std::forward<Month>(month)}
            , year_{std::forward<Year>(year)}
        {
        }

        constexpr date_literal(date_literal const&) = default;

        constexpr date_literal(date_literal&&) noexcept = default;

    public: // Destruction
        constexpr ~date_literal() = default;

    public: // Interface
        [[nodiscard]] constexpr uint8_t day() const { return day_; }

        [[nodiscard]] constexpr uint8_t month() const { return month_; }

        [[nodiscard]] constexpr int64_t year() const { return year_; }

    public: // Operators
        constexpr date_literal& operator=(date_literal const&) = default;

        constexpr date_literal& operator=(date_literal&&) noexcept = default;

        friend constexpr bool operator==(date_literal const&,
            date_literal const&) = default;

    private: // Data
        uint8_t day_;
        uint8_t month_;
        int64_t year_;
    };

    struct [[nodiscard]] time_zone_literal final : public sign_mixin<true>
    {
    public: // Construction
        template<typename Sign, typename Hours, typename Minutes>
        constexpr time_zone_literal(Sign&& sign,
            Hours&& hours,
            Minutes&& minutes)
        requires(std::constructible_from<sign_mixin<true>, Sign> &&
                    std::constructible_from<uint8_t, Hours> &&
                    std::constructible_from<uint8_t, Minutes>)
            : sign_mixin{std::forward<Sign>(sign)}
            , hours_{std::forward<Hours>(hours)}
            , minutes_{std::forward<Minutes>(minutes)}
        {
        }

        constexpr time_zone_literal(time_zone_literal const&) = default;

        constexpr time_zone_literal(time_zone_literal&&) noexcept = default;

    public: // Destruction
        constexpr ~time_zone_literal() = default;

    public: // Interface
        [[nodiscard]] constexpr uint8_t hours() const { return hours_; }

        [[nodiscard]] constexpr uint8_t minutes() const { return minutes_; }

    public: // Operators
        constexpr time_zone_literal& operator=(
            time_zone_literal const&) = default;

        constexpr time_zone_literal& operator=(
            time_zone_literal&&) noexcept = default;

        friend constexpr bool operator==(time_zone_literal const&,
            time_zone_literal const&) = default;

    private: // Data
        uint8_t hours_;
        uint8_t minutes_;
    };

    struct [[nodiscard]] time_literal final
    {
    public: // Construction
        template<typename Hours,
            typename Minutes,
            typename Seconds,
            typename Fraction = uint32_t,
            typename TimeZone = std::optional<time_zone_literal>>
        constexpr time_literal(Hours&& hours,
            Minutes&& minutes,
            Seconds&& seconds,
            Fraction&& fraction = 0,
            TimeZone&& tz = std::nullopt)
        requires(std::constructible_from<uint8_t, Hours> &&
                    std::constructible_from<uint8_t, Minutes> &&
                    std::constructible_from<uint8_t, Seconds> &&
                    std::constructible_from<uint32_t, Fraction> &&
                    std::constructible_from<std::optional<time_zone_literal>,
                        TimeZone>)
            : hours_{std::forward<Hours>(hours)}
            , minutes_{std::forward<Minutes>(minutes)}
            , seconds_{std::forward<Seconds>(seconds)}
            , fraction_{std::forward<Fraction>(fraction)}
            , time_zone_{std::forward<TimeZone>(tz)}
        {
        }

        constexpr time_literal(time_literal const&) = default;

        constexpr time_literal(time_literal&&) noexcept = default;

    public: // Destruction
        constexpr ~time_literal() = default;

    public: // Interface
        [[nodiscard]] constexpr uint8_t hours() const { return hours_; }

        [[nodiscard]] constexpr uint8_t minutes() const { return minutes_; }

        [[nodiscard]] constexpr uint8_t seconds() const { return seconds_; }

        [[nodiscard]] constexpr uint32_t fraction() const { return fraction_; }

        [[nodiscard]] constexpr std::optional<time_zone_literal>
        time_zone() const
        {
            return time_zone_;
        }

    public: // Operators
        constexpr time_literal& operator=(time_literal const&) = default;

        constexpr time_literal& operator=(time_literal&&) noexcept = default;

        friend constexpr bool operator==(time_literal const&,
            time_literal const&) = default;

    private: // Data
        uint8_t hours_;
        uint8_t minutes_;
        uint8_t seconds_;
        uint32_t fraction_;
        std::optional<struct time_zone_literal> time_zone_;
    };

    struct [[nodiscard]] timestamp_literal final
    {
    public: // Construction
        template<typename Date, typename Time>
        constexpr timestamp_literal(Date&& d, Time&& t)
        requires(std::constructible_from<struct date_literal, Date> &&
                    std::constructible_from<struct time_literal, Time>)
            : date_{std::forward<Date>(d)}
            , time_{std::forward<Time>(t)}
        {
        }

        constexpr timestamp_literal(
            timestamp_literal const&) noexcept = default;

        constexpr timestamp_literal(timestamp_literal&&) noexcept = default;

    public: // Destruction
        constexpr ~timestamp_literal() = default;

    public: // Interface
        [[nodiscard]] constexpr struct date_literal const& date() const&
        {
            return date_;
        }

        [[nodiscard]] constexpr struct date_literal date() &&
        {
            return std::move(date_);
        }

        [[nodiscard]] constexpr struct time_literal const& time() const&
        {
            return time_;
        }

        [[nodiscard]] constexpr struct time_literal time() &&
        {
            return std::move(time_);
        }

    public: // Operators
        constexpr timestamp_literal& operator=(
            timestamp_literal const&) = default;

        constexpr timestamp_literal& operator=(timestamp_literal&&) = default;

        friend constexpr bool operator==(timestamp_literal const&,
            timestamp_literal const&) = default;

    private: // Data
        struct date_literal date_;
        struct time_literal time_;
    };

    struct [[nodiscard]] datetime_literal
    {
    public: // Types
        using underlying_type =
            std::variant<date_literal, time_literal, timestamp_literal>;

    public: // Construction
        template<typename Value>
        constexpr datetime_literal(Value&& value)
        requires(std::constructible_from<underlying_type, Value>)
            : value_{std::forward<Value>(value)}
        {
        }

        constexpr datetime_literal(datetime_literal const&) = default;

        constexpr datetime_literal(datetime_literal&&) noexcept = default;

    public: // Destruction
        constexpr ~datetime_literal() = default;

    public: // Interface
        template<typename T>
        [[nodiscard]] constexpr bool of_type() const
        {
            return std::holds_alternative<T>(value_);
        }

        [[nodiscard]] constexpr std::string_view contained_type() const
        {
            if (of_type<date_literal>())
            {
                return "date_literal";
            }
            else if (of_type<time_literal>())
            {
                return "time_literal";
            }
            else
            {
                return "timestamp_literal";
            }
        }

        [[nodiscard]] constexpr underlying_type const& value() const&
        {
            return value_;
        }

        [[nodiscard]] constexpr underlying_type value() &&
        {
            return std::move(value_);
        }

    public: // Operators
        constexpr datetime_literal& operator=(
            datetime_literal const&) = default;

        constexpr datetime_literal& operator=(
            datetime_literal&&) noexcept = default;

        friend constexpr bool operator==(datetime_literal const&,
            datetime_literal const&) = default;

    public: // Data
        underlying_type value_;
    };
} // namespace melinda::mqlast

#endif
