#include <mbltrc_timestamp.h>

#include <chrono>

#include <date/date.h>

melinda::mbltrc::timestamp melinda::mbltrc::current_timestamp() noexcept
{
    using namespace date;
    std::chrono::system_clock::time_point const time =
        std::chrono::system_clock::now();

    auto const daypoint = floor<days>(time);
    year_month_day const ymd = year_month_day(daypoint);

    auto const tmd = make_time(time - daypoint);

    std::chrono::duration tp =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            time.time_since_epoch());
    tp -= std::chrono::duration_cast<std::chrono::seconds>(tp);

    return {static_cast<int16_t>(static_cast<int>(ymd.year())),
        static_cast<uint8_t>(static_cast<unsigned>(ymd.month())),
        static_cast<uint8_t>(static_cast<unsigned>(ymd.day())),
        static_cast<uint8_t>(tmd.hours().count()),
        static_cast<uint8_t>(tmd.minutes().count()),
        static_cast<uint8_t>(tmd.seconds().count()),
        static_cast<uint16_t>(tp / std::chrono::milliseconds(1))};
}
