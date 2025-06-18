#pragma once

#include "point.h"

#include <ostream>

namespace orpg
{
   struct extents
   {
      using value_t = std::uint16_t;

      value_t width{0};
      value_t height{0};

#if __cpp_lib_three_way_comparison

      [[nodiscard]] constexpr bool operator==(extents const& other) const = default;

      [[nodiscard]] constexpr bool operator!=(extents const& other) const = default;

#else
      [[nodiscard]] constexpr bool operator==(extents const& other) const
      {
         return std::tie(width, height) == std::tie(other.width, other.height);
      }

      [[nodiscard]] constexpr bool operator!=(extents const& other) const { return !(*this == other); }
#endif

      //! \todo write test
      [[nodiscard]] constexpr auto operator/(std::integral auto rhs) const noexcept -> extents
      {
         return {.width = static_cast<value_t>(width / rhs), .height = static_cast<value_t>(height / rhs)};
      }

      //! \todo write test
      [[nodiscard]] constexpr auto center() const noexcept -> point
      {
         return {.x = static_cast<point::value_t>(width / 2), .y = static_cast<point::value_t>(height / 2)};
      }

      friend auto operator<<(std::ostream& os, extents const& obj) -> decltype(auto)
      {
         return os << '[' << obj.width << 'x' << obj.height << ']';
      }
   };
} // namespace orpg
