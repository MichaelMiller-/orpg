#pragma once

#include "point.h"

// #ifndef NDEBUG
#include <ostream>
// #endif

namespace orpg {
    struct extents {
        using value_t = std::uint16_t;
        value_t width{0};
        value_t height{0};

#if __cpp_lib_three_way_comparison

        [[nodiscard]] constexpr bool operator==(extents const &other) const = default;

        [[nodiscard]] constexpr bool operator!=(extents const &other) const = default;

#else
        [[nodiscard]] constexpr bool operator==(extents const &other) const {
            return std::tie(width, height) == std::tie(other.width, other.height);
        }

        [[nodiscard]] constexpr bool operator!=(extents const &other) const {
            return !(*this == other);
        }
#endif

        //! \todo write test
        template<typename T>
        [[nodiscard]] constexpr auto operator/(T rhs) const noexcept {
            return extents{static_cast<value_t>(width / rhs), static_cast<value_t>(height / rhs)};
        }

        //! \todo write test
        [[nodiscard]] constexpr auto center() const noexcept -> point {
            return {width / 2, height / 2};
        }

// #ifndef NDEBUG
        template<typename CharT, typename Traits>
        friend auto &operator<<(std::basic_ostream<CharT, Traits> &os, extents const &obj) {
            return os << '[' << obj.width << 'x' << obj.height << ']';
        }
// #endif
    };
}
