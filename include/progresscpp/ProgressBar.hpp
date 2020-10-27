#pragma once

#include <chrono>
#include <iostream>

namespace progresscpp {

template<class T>
class ProgressBar
{
public:
    using number_type = T;

    ProgressBar(number_type total,
                number_type width,
                char complete,
                char incomplete) noexcept
        : total_ticks_{total},
          bar_width_{width},
          complete_char_{complete},
          incomplete_char_{incomplete} {}

    ProgressBar(number_type total, number_type width) noexcept
        : total_ticks_{total}, bar_width_{width} {}

    ProgressBar(ProgressBar&&) noexcept = default;
    ProgressBar(const ProgressBar&&) = delete;

    auto operator=(ProgressBar&&) noexcept -> ProgressBar& = default;
    auto operator=(const ProgressBar&) -> ProgressBar& = default;

    auto operator++() noexcept
        -> number_type
    {
        ticks_ = std::min(total_ticks_, ticks_ + 1);
        return ticks_;
    }

    auto operator++(int) noexcept
        -> number_type
    {
        ticks_ = std::min(total_ticks_, ticks_ + 1);
        return ticks_;
    }

    auto operator+=(number_type i) noexcept
        -> ProgressBar&
    {
        ticks_ = std::min(total_ticks_, ticks_ + i);
        return *this;
    }

    auto display() const noexcept
        -> void
    {
        auto progress = static_cast<float>(ticks_) / total_ticks_;
        auto pos = static_cast<number_type>(bar_width_ * progress);

        auto time_elapsed = millisecondSinceStart();

        std::cout << "[";

        for(number_type i{0}; i < bar_width_; ++i) {
            if(i < pos)
                std::cout << complete_char_;
            else if(i == pos)
                std::cout << ">";
            else
                std::cout << incomplete_char_;
        }

        std::cout << "] "
                  << static_cast<int>(progress * 100.0)
                  << "% "
                  << static_cast<float>(time_elapsed) / 1000.0
                  << "s\r";

        std::cout.flush();
    }

    auto millisecondSinceStart() const noexcept
        -> long
    {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_)
            .count();
    }

    auto done() noexcept
        -> void
    {
        ticks_ = total_ticks_;
        display();
        std::cout << std::endl;
    }

private:
    number_type ticks_ = 0;

    const number_type total_ticks_;
    const number_type bar_width_;
    const char complete_char_ = '=';
    const char incomplete_char_ = ' ';
    const std::chrono::steady_clock::time_point start_time_ =
        std::chrono::steady_clock::now();
};

} // namespace progresscpp
