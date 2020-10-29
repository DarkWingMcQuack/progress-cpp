#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string_view>

namespace progresscpp {

inline auto msToString(long ms)
    -> std::string
{
    auto h = ms / (1000 * 60 * 60);
    ms -= h * (1000 * 60 * 60);

    auto m = ms / (1000 * 60);
    ms -= m * (1000 * 60);

    auto s = ms / 1000;
    ms -= s * 1000;

    std::stringstream ss;

    ss << std::setfill('0')
       << std::setw(2)
       << h
       << ':'
       << std::setw(2)
       << m
       << ':'
       << std::setw(2)
       << s
       << '.'
       << std::setw(3)
       << ms;

    return ss.str();
}


template<class T>
class ProgressBar
{
public:
    using number_type = T;

    ProgressBar(number_type total,
                number_type width,
                char complete = '=',
                char incomplete = ' ',
                char arrow = '>') noexcept
        : total_ticks_{total},
          bar_width_{width},
          complete_char_{complete},
          incomplete_char_{incomplete},
          arrow_char_{arrow} {}

    ProgressBar(ProgressBar&&) noexcept = default;
    ProgressBar(const ProgressBar&&) = delete;

    auto operator=(ProgressBar&&) noexcept -> ProgressBar& = default;
    auto operator=(const ProgressBar&) -> ProgressBar& = delete;

    auto operator++() noexcept
        -> number_type
    {
        *this += 1;
        return ticks_;
    }

    auto operator++(int) noexcept
        -> number_type
    {
        *this += 1;
        return ticks_;
    }

    auto operator+=(number_type i) noexcept
        -> ProgressBar&
    {
        ticks_ = std::min(total_ticks_, ticks_ + i);
        return *this;
    }

    auto display() noexcept
        -> void
    {
        drawed_ticks_ = ticks_;

        auto progress = static_cast<double>(ticks_) / total_ticks_;
        auto pos = static_cast<number_type>(bar_width_ * progress);

        auto time_elapsed = millisecondSinceStart();

        std::cout << "["
                  << std::setfill(complete_char_)
                  << std::setw(pos)
                  << (pos == bar_width_ ? '\0' : arrow_char_)
                  << std::setfill(incomplete_char_)
                  << std::setw(bar_width_ - pos + 1)
                  << "] "
                  << " "
                  << std::fixed
                  << std::setprecision(2)
                  << static_cast<int>(progress * 100.0)
                  << "% ["
                  << msToString(static_cast<double>(time_elapsed)) // / 1000.0
                  << "s, ETA: "
                  << msToString(calculateETA())
                  << "]s\r"
                  << std::flush;
    }

    // displays the bar only if it was changed at least by the given
    // amount of percent since it was last displayed
    auto displayIfChangedAtLeast(double percent) noexcept
        -> void
    {
        auto new_progress = static_cast<float>(ticks_) / total_ticks_;
        auto old_progress = static_cast<float>(drawed_ticks_) / total_ticks_;

        if(new_progress - old_progress >= percent) {
            display();
        }
    }

    auto millisecondSinceStart() const noexcept
        -> long
    {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_)
            .count();
    }

    auto restart() noexcept
        -> void
    {
        ticks_ = 0;
        start_time_ = std::chrono::steady_clock::now();
    }

    auto done() noexcept
        -> void
    {
        ticks_ = total_ticks_;
        auto time_elapsed = millisecondSinceStart();
        std::cout << "\r\033[1;32m"
                  << "["
                  << std::setfill(complete_char_)
                  << std::setw(bar_width_ + 1)
                  << "] "
                  << 100
                  << "% ["
                  << msToString(static_cast<double>(time_elapsed)) // / 1000.0
                  << "s]"
                  << std::setfill(' ')
                  << std::setw(25)
                  << "\033[0m"
                  << std::endl;
    }

    auto failure(std::string_view error_message = "") noexcept
        -> void
    {
        std::cout << "\033[1;31m";

        auto progress = static_cast<double>(ticks_) / total_ticks_;
        auto pos = static_cast<number_type>(bar_width_ * progress);

        auto time_elapsed = millisecondSinceStart();

        std::cout << "["
                  << std::setfill(complete_char_)
                  << std::setw(pos)
                  << (pos == bar_width_ ? '\0' : arrow_char_)
                  << std::setfill(incomplete_char_)
                  << std::setw(bar_width_ - pos + 1)
                  << "] "
                  << " "
                  << std::fixed
                  << std::setprecision(2)
                  << static_cast<int>(progress * 100.0)
                  << "% ["
                  << msToString(static_cast<double>(time_elapsed)) // / 1000.0
                  << "s, Failure"
                  << "]"
                  << std::setfill(' ')
                  << std::setw(12)
                  << (error_message.empty() ? '\0' : '\n')
                  << error_message
                  << "\033[0m"
                  << std::endl;
    }

private:
    auto calculateETA() const noexcept
        -> long
    {
        auto progress = static_cast<float>(ticks_) / total_ticks_;
        auto since_start = millisecondSinceStart();
        return since_start / (progress * 100) * ((1 - progress) * 100);
    }

private:
    number_type drawed_ticks_ = 0;
    number_type ticks_ = 0;
    std::chrono::steady_clock::time_point start_time_ =
        std::chrono::steady_clock::now();

    const number_type total_ticks_;
    const number_type bar_width_;
    const char complete_char_;
    const char incomplete_char_;
    const char arrow_char_;
};

} // namespace progresscpp
