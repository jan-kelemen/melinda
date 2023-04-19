#include <mbltrc_memory_mapped_sink.h>

#include <atomic>
#include <iostream>
#include <mutex>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <fmt/std.h>
#include <source_location>
#include <thread>

namespace
{
    [[nodiscard]] constexpr std::string_view file_name_only(
        std::string_view file_name)
    {
        if (auto last_sep{file_name.find_last_of(
                std::filesystem::path::preferred_separator)};
            last_sep != std::string_view::npos)
        {
            return file_name.substr(last_sep + 1);
        }
        return file_name;
    }
} // namespace

class [[nodiscard]] melinda::mbltrc::memory_mapped_sink::impl final
{
public: // Construction
    impl(trace_level initial_level,
        std::filesystem::path const& directory,
        std::filesystem::path const& base_filename,
        std::size_t file_size);

public: // Destruction
    ~impl() noexcept;

public: // Interface
    void change_level(trace_level new_level) noexcept;

    bool should_trace_message(trace_level level) const noexcept;

    void trace(message_origin const& origin,
        timestamp const& time,
        trace_level level,
        std::string_view message) const noexcept;

private:
    boost::iostreams::mapped_file_params new_file_params() const;

private: // Data
    std::filesystem::path base_path_;
    std::size_t file_size_;

    std::atomic<trace_level> level_;
    mutable std::mutex mtx_;
    mutable boost::iostreams::stream<boost::iostreams::mapped_file_sink> file_;
    mutable std::size_t bytes_written_{0};
};

melinda::mbltrc::memory_mapped_sink::impl::impl(
    melinda::mbltrc::trace_level initial_level,
    std::filesystem::path const& directory,
    std::filesystem::path const& base_filename,
    std::size_t file_size)
    : base_path_{directory / base_filename}
    , file_size_{file_size}
    , level_{initial_level}
{
    create_directories(directory);

    file_.open(new_file_params());
}

void melinda::mbltrc::memory_mapped_sink::impl::change_level(
    melinda::mbltrc::trace_level new_level) noexcept
{
    level_.store(new_level, std::memory_order_release);
}

bool melinda::mbltrc::memory_mapped_sink::impl::should_trace_message(
    melinda::mbltrc::trace_level level) const noexcept
{
    return level >= level_.load(std::memory_order_consume);
}

void melinda::mbltrc::memory_mapped_sink::impl::trace(
    melinda::mbltrc::message_origin const& origin,
    melinda::mbltrc::timestamp const& time,
    melinda::mbltrc::trace_level level,
    std::string_view message) const noexcept
{
    std::string full_message{fmt::format("{} [{}] {} {} [{}:{}]\n",
        time,
        level,
        origin.thread_id,
        message,
        file_name_only(origin.source.file_name()),
        origin.source.line())};

    std::lock_guard g{mtx_};

    try
    {
        if (full_message.size() >= file_size_ - bytes_written_)
        {
            file_.close();

            file_.open(new_file_params());
            bytes_written_ = 0;
        }

        file_ << full_message;
        bytes_written_ += full_message.size();
    }
    catch (std::ios::failure const& ex)
    {
        timestamp now{current_timestamp()};
        std::source_location location{std::source_location::current()};

        std::cerr << fmt::format(
            "{} [{}] {} Error while writing message to log file {} {} [{}:{}]\n{}",
            now,
            trace_level::error,
            std::this_thread::get_id(),
            ex.code().value(),
            ex.what(),
            file_name_only(location.file_name()),
            location.line(),
            full_message);
    }
}

boost::iostreams::mapped_file_params
melinda::mbltrc::memory_mapped_sink::impl::new_file_params() const
{
    timestamp const now{current_timestamp()};
    std::string suffix{fmt::format("_{:04}{:02}{:02}T{:02}{:02}{:02}{:03}",
        now.year,
        now.month,
        now.day,
        now.hours,
        now.minutes,
        now.seconds,
        now.milliseconds)};

    std::filesystem::path name{base_path_};
    name += suffix;
    name.replace_extension("log");

    boost::iostreams::mapped_file_params rv{name};
    rv.flags = boost::iostreams::mapped_file::readwrite;
    rv.new_file_size =
        boost::numeric_cast<boost::iostreams::stream_offset>(file_size_);

    return rv;
}

melinda::mbltrc::memory_mapped_sink::impl::~impl() noexcept
{
    try
    {
        file_->resize(boost::numeric_cast<boost::iostreams::stream_offset>(
            bytes_written_));
    }
    catch (std::iostream::failure const& ex)
    {
        timestamp now{current_timestamp()};
        std::source_location location{std::source_location::current()};

        std::cerr << fmt::format(
            "{} [{}] {} Error while closing log file {} {} [{}:{}]\n",
            now,
            trace_level::error,
            std::this_thread::get_id(),
            ex.code().value(),
            ex.what(),
            location.file_name(),
            location.line());
    }
}

melinda::mbltrc::memory_mapped_sink::memory_mapped_sink(
    melinda::mbltrc::trace_level initial_level,
    std::filesystem::path const& directory,
    std::filesystem::path const& base_filename,
    std::size_t file_size)
    : impl_{std::make_unique<impl>(initial_level,
          directory,
          base_filename,
          file_size)}
{
}

void melinda::mbltrc::memory_mapped_sink::change_level(
    melinda::mbltrc::trace_level new_level) noexcept
{
    impl_->change_level(new_level);
}

bool melinda::mbltrc::memory_mapped_sink::should_trace_message(
    melinda::mbltrc::trace_level level) const noexcept
{
    return impl_->should_trace_message(level);
}

void melinda::mbltrc::memory_mapped_sink::trace(
    melinda::mbltrc::message_origin const& origin,
    melinda::mbltrc::timestamp const& time,
    melinda::mbltrc::trace_level level,
    std::string_view message) const noexcept
{
    impl_->trace(origin, time, level, message);
}

melinda::mbltrc::memory_mapped_sink::~memory_mapped_sink() noexcept = default;
