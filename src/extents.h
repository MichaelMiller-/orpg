#pragma once

#include "point.h"

// #ifndef NDEBUG
#include <ostream>
// #endif

namespace orpg
{
    struct extents
    {
        using value_t = std::uint16_t;
        value_t width{0};
        value_t height{0};

        [[nodiscard]] constexpr bool operator==(extents const &other) const = default;

        [[nodiscard]] constexpr bool operator!=(extents const &other) const = default;

        //! \todo write test
        template<typename T>
        [[nodiscard]] constexpr auto operator / (T rhs) const noexcept
        {
            return extents{static_cast<value_t>(width / rhs), static_cast<value_t>(height / rhs) };
        }

        //! \todo write test
        [[nodiscard]] constexpr auto center() const noexcept -> point
        {
            return { static_cast<decltype(point::x)>(width / 2), static_cast<decltype(point::y)>(height / 2) };
        }
// #ifndef NDEBUG
        template <typename CharT, typename Traits>
        friend auto& operator<<(std::basic_ostream<CharT, Traits> &os, extents const &obj) {
            return os << '[' << obj.width << 'x' << obj.height << ']';
        }
// #endif
    };
}
