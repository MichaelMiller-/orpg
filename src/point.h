#pragma once

#include <cstdint>
#include <compare>
// #ifndef NDEBUG
#include <ostream>
#include <sstream>
// #endif

namespace orpg
{
    struct point
    {
        using value_t = std::int32_t;
        value_t x{0};
        value_t y{0};

        [[nodiscard]] constexpr auto operator<=>(point const &other) const = default;

// #ifndef NDEBUG
        template <typename CharT, typename Traits>
        friend auto& operator<<(std::basic_ostream<CharT, Traits> &os, point const &obj) {
            return os << '(' << obj.x << ',' << obj.y << ')';
        }
// #endif
    };

// #ifndef NDEBUG
    [[nodiscard]] auto to_string(point const& obj)
    {
        std::stringstream result{};
        result << obj;
        return result.str();
    }
// #endif

    [[nodiscard]] constexpr auto operator+(point const&lhs, point const &rhs) noexcept -> point {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }

    [[nodiscard]] constexpr auto operator-(point const&lhs, point const& rhs) noexcept -> point {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }

    template <typename T>
    [[nodiscard]] constexpr auto operator / (point const&lhs, T const& rhs) noexcept -> point {
        return {lhs.x / rhs, lhs.y / rhs};
    }
}
