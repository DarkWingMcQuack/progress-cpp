#pragma once

#include <chrono>
#include <iostream>

namespace progresscpp {

class ProgressBar
{
private:
    std::size_t ticks_ = 0;

    const std::size_t total_ticks_;
    const std::size_t bar_width_;
    const char complete_char_ = '=';
    const char incomplete_char_ = ' ';
    const std::chrono::steady_clock::time_point start_time_ =
        std::chrono::steady_clock::now();

public:
    ProgressBar(std::size_t total, std::size_t width, char complete,
                char incomplete) noexcept
        : total_ticks_{total},
          bar_width_{width},
          complete_char_{complete},
          incomplete_char_{incomplete} {}

    ProgressBar(std::size_t total, std::size_t width) noexcept
        : total_ticks_{total}, bar_width_{width} {}

    auto operator++() noexcept
        -> std::size_t
    {
        ticks_ = std::min(total_ticks_, ticks_ + 1);
        return ticks_;
    }

    auto operator++(int) noexcept
        -> std::size_t
    {
        ticks_ = std::min(total_ticks_, ticks_ + 1);
        return ticks_;
    }

    auto operator+=(std::size_t i) noexcept
        -> ProgressBar&
    {
        ticks_ = std::min(total_ticks_, ticks_ + i);
        return *this;
    }

    auto display() const noexcept
        -> void
    {
        auto progress = static_cast<float>(ticks_) / total_ticks_;
        auto pos = static_cast<std::size_t>(bar_width_ * progress);

        auto now = std::chrono::steady_clock::now();
        auto time_elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_)
                .count();

        std::cout << "[";

        for(size_t i{0}; i < bar_width_; ++i) {
            if(i < pos)
                std::cout << complete_char_;
            else if(i == pos)
                std::cout << ">";
            else
                std::cout << incomplete_char_;
        }

        std::cout << "] " << int(progress * 100.0) << "% "
                  << float(time_elapsed) / 1000.0 << "s\r";
        std::cout.flush();
    }

    auto done() noexcept
        -> void
    {
        ticks_ = total_ticks_;
        display();
        std::cout << std::endl;
    }
};

} // namespace progresscpp
