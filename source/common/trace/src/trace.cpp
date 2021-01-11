#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <date/date.h>

#include "scope_exit.h"

#include "trace.h"

namespace
{
    std::unique_ptr<mel::trace::trace_handle> process_trace_handle;
}

class [[nodiscard]] mel::trace::trace_handle::impl final
{
public: // Types
    using class_name = impl;

public: // Construction
    impl() = delete;

    impl(trace_options const& options);

    impl(impl const&) = delete;

    impl(impl&& other) noexcept = delete;

public: // Interface
    [[nodiscard]] auto should_trace_message(
        enum trace_level level) const noexcept -> bool;

    void trace_level(enum trace_level new_level) noexcept;

    void trace(std::string_view message) const noexcept;

public: // Operators
    impl& operator=(impl const&) = delete;

    impl& operator=(impl&&) noexcept = delete;

public: // Destruction
    ~impl() noexcept;

private: // Constants
    static constexpr int bad_file_descriptor = -1;

    static constexpr std::size_t delay_filesize_check_default = 50;

private: // Helpers
    [[nodiscard]] bool has_filesize_exceeded(
        int file_descriptor_) const noexcept;

    [[nodiscard]] fs::path new_filename() const;

    [[nodiscard]] int open_file(fs::path const& full_path) const;

    [[nodiscard]] int open_latest_file() const;

    [[nodiscard]] std::vector<fs::path> find_matching_files() const;

private: // Data
    mutable std::timed_mutex mtx;

    fs::path const directory_;

    fs::path const base_filename_;

    uintmax_t const max_filesize_;

    uint8_t const log_files_to_keep_;

    bool const flush_on_write_;

    std::atomic<enum trace_level> trace_level_;

    mutable int file_descriptor_ = bad_file_descriptor;

    mutable std::size_t delay_filesize_check_ = 0;
};

mel::trace::trace_handle::impl::impl(trace_options const& options)
    : directory_(fs::absolute(options.trace_directory))
    , base_filename_(options.base_filename)
    , max_filesize_(options.max_filesize)
    , log_files_to_keep_(options.log_files_to_keep)
    , flush_on_write_(options.flush_on_write)
    , trace_level_(options.level)
    , file_descriptor_(open_latest_file())
{
}

bool mel::trace::trace_handle::impl::should_trace_message(
    enum trace_level level) const noexcept
{
    return level >= trace_level_.load();
}

void mel::trace::trace_handle::impl::trace_level(
    enum trace_level new_level) noexcept
{
    trace_level_.store(new_level);
}

void mel::trace::trace_handle::impl::trace(
    std::string_view message) const noexcept
{
    using namespace std::chrono_literals;
    if (!mtx.try_lock_for(5s))
    {
        return;
    }
    ON_SCOPE_EXIT(mtx.unlock());

    if (write(file_descriptor_, message.data(), message.length()) < 0)
    {
        return;
    }

    if (!static_cast<bool>(max_filesize_))
    {
        return;
    }

    if (++delay_filesize_check_ < delay_filesize_check_default)
    {
        return;
    }
    delay_filesize_check_ = 0;

    if (!has_filesize_exceeded(file_descriptor_))
    {
        return;
    }

    int const new_descriptor = open_file(new_filename());
    if (new_descriptor == bad_file_descriptor)
    {
        return;
    }

    if (!flush_on_write_)
    {
        fsync(file_descriptor_);
    }
    close(std::exchange(file_descriptor_, new_descriptor));
    delay_filesize_check_ = 0;

    if (!static_cast<bool>(log_files_to_keep_))
    {
        return;
    }

    std::vector<fs::path> matching_entries = find_matching_files();
    if (matching_entries.size() <= log_files_to_keep_)
    {
        return;
    }

    std::nth_element(std::begin(matching_entries),
        std::begin(matching_entries) + log_files_to_keep_,
        std::end(matching_entries),
        [](fs::path const& lhs, fs::path const& rhs) {
            return fs::last_write_time(lhs) > fs::last_write_time(rhs);
        });

    std::for_each(std::cbegin(matching_entries) + log_files_to_keep_,
        std::cend(matching_entries),
        [](fs::path const& p) { fs::remove(p); });
}

bool mel::trace::trace_handle::impl::has_filesize_exceeded(
    int const file_descriptor) const noexcept
{
    struct stat64 file_stats
    {
    };
    bool const have_filesize =
        fstat64(file_descriptor, std::addressof(file_stats)) == 0;

    // Just assume that the file limit wasn't exceeded if no filesize can be
    // retrieved.
    return have_filesize &&
        static_cast<uintmax_t>(file_stats.st_size) >= max_filesize_;
}

fs::path mel::trace::trace_handle::impl::new_filename() const
{
    std::time_t const current_time =
        std::chrono::high_resolution_clock::to_time_t(
            std::chrono::high_resolution_clock::now());
    struct tm tm = {};
    gmtime_r(std::addressof(current_time), std::addressof(tm));
    std::array<char, sizeof("_20200201204856.log") + 1> buffer = {};
    strftime(buffer.data(),
        buffer.size(),
        "_%Y%m%d%H%M%S.log",
        std::addressof(tm));

    fs::path base_filename = base_filename_;
    base_filename.concat(buffer.data());
    fs::path const& filename = base_filename;

    return directory_ / filename;
}

int mel::trace::trace_handle::impl::open_file(fs::path const& full_path) const
{
    fs::path const directory = full_path.parent_path();
    if (!fs::exists(directory))
    {
        fs::create_directory(directory);
    }

    int const sync_flag = flush_on_write_ ? O_SYNC : 0;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
    return open(full_path.native().c_str(),
        O_CREAT | O_WRONLY | O_APPEND | sync_flag,
        S_IRUSR | S_IWUSR);
}

int mel::trace::trace_handle::impl::open_latest_file() const
{
    std::vector<fs::path> const matching_entries = find_matching_files();
    if (matching_entries.empty())
    {
        return open_file(new_filename());
    }

    fs::path const& p = *std::max_element(std::cbegin(matching_entries),
        std::cend(matching_entries),
        [](fs::path const& lhs, fs::path const& rhs) {
            return fs::last_write_time(lhs) < fs::last_write_time(rhs);
        });

    int const file_descriptor = open_file(p);
    if (file_descriptor != bad_file_descriptor &&
        !has_filesize_exceeded(file_descriptor))
    {
        return file_descriptor;
    }
    return open_file(new_filename());
}

std::vector<fs::path>
mel::trace::trace_handle::impl::find_matching_files() const
{
    std::vector<fs::path> matching_entries;
    if (!fs::exists(directory_))
    {
        return matching_entries;
    }

    matching_entries.reserve(log_files_to_keep_);
    for (fs::directory_entry const& entry : fs::directory_iterator(directory_))
    {
        bool const regular_file = fs::is_regular_file(entry.status());
        bool const matches_stem = entry.path().filename().native().compare(
                                      base_filename_.native()) > 0;

        if (regular_file && matches_stem)
        {
            matching_entries.emplace_back(entry.path());
        }
    }
    return matching_entries;
}

mel::trace::trace_handle::impl::~impl() noexcept
{
    if (file_descriptor_ != bad_file_descriptor)
    {
        if (!flush_on_write_)
        {
            fsync(file_descriptor_);
        }
        close(file_descriptor_);
        file_descriptor_ = bad_file_descriptor;
    }
}

mel::trace::trace_options::trace_options(fs::path trace_directory,
    fs::path base_filename)
    : trace_directory(std::move(trace_directory))
    , base_filename(std::move(base_filename))
{
}

mel::trace::trace_handle::trace_handle() noexcept : impl_(nullptr) { }

mel::trace::trace_handle::trace_handle(trace_options const& options)
    : impl_(std::make_shared<impl>(options))
{
}

bool mel::trace::trace_handle::should_trace_message(
    enum trace_level const level) const noexcept
{
    return impl_ && impl_->should_trace_message(level);
}

void mel::trace::trace_handle::trace_level(enum trace_level new_level) noexcept
{
    if (impl_)
    {
        impl_->trace_level(new_level);
    }
}

void mel::trace::trace_handle::trace(std::string_view message) const noexcept
{
    if (impl_)
    {
        impl_->trace(message);
    }
}

mel::trace::trace_handle::~trace_handle() noexcept = default;

mel::trace::trace_handle mel::trace::create_trace_handle(
    mel::trace::trace_options const& options)
{
    try
    {
        return mel::trace::trace_handle(options);
    }
    catch (std::exception const& ex)
    {
        // TODO-JK: figure out how to report this error?
        std::cout << ex.what() << '\n';
        return mel::trace::trace_handle();
    }
}

void mel::trace::initialize_process_trace_handle(
    mel::trace::trace_handle&& handle)
{
    process_trace_handle =
        std::make_unique<mel::trace::trace_handle>(std::move(handle));
}

void close_trace(mel::trace::trace_handle& handle) noexcept
{
    handle = mel::trace::trace_handle();
}

mel::trace::detail::timestamp mel::trace::detail::current_timestamp() noexcept
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

mel::trace::trace_handle const*
mel::trace::detail::process_trace_handle() noexcept
{
    return ::process_trace_handle.get();
}
