#pragma once

#include "extents.h"
#include "point.h"

#include <ostream>
#include <sstream>

namespace orpg
{
   struct rect
   {
      point top_left_position{};
      extents extent{};

      [[nodiscard]] constexpr auto left() const noexcept { return top_left_position.x; }

      [[nodiscard]] constexpr auto top() const noexcept { return top_left_position.y; }

      [[nodiscard]] constexpr auto bottom() const noexcept { return top_left_position.y + extent.height; }

      [[nodiscard]] constexpr auto right() const noexcept { return top_left_position.x + extent.width; }

      [[nodiscard]] constexpr point const& top_left() const noexcept { return top_left_position; }

      [[nodiscard]] constexpr point bottom_right() const noexcept { return {right(), bottom()}; }

      [[nodiscard]] constexpr point bottom_left() const noexcept { return {left(), bottom()}; }

      [[nodiscard]] constexpr point top_right() const noexcept { return {right(), top()}; }

      [[nodiscard]] constexpr auto width() const noexcept { return extent.width; }

      [[nodiscard]] constexpr auto height() const noexcept { return extent.height; }

      // returns a rectangle of this extents, but centered in s
      // precondition: extents s must be greater than this extents
      [[nodiscard]] constexpr auto centered(extents const& s) const noexcept -> rect
      {
         //! \todo error handling
         return {{(s.width - extent.width) / 2, (s.height - extent.height) / 2}, extent};
      }

      [[nodiscard]] constexpr bool intersects(rect const& other) const noexcept
      {
         const auto my_tl = top_left();
         const auto my_br = bottom_right();
         const auto other_tl = other.top_left();
         const auto other_br = other.bottom_right();

         return my_tl.x < other_br.x && my_br.x > other_tl.x && my_tl.y < other_br.y && my_br.y > other_tl.y;
      };

      [[nodiscard]] constexpr auto center() const noexcept -> point { return extent.center() + top_left_position; }

      [[nodiscard]] constexpr auto is_inside(point pt) const noexcept -> bool
      {
         return pt.x >= top_left_position.x && pt.y >= top_left_position.y &&
                pt.x < (top_left_position.x + extent.width) && pt.y < (top_left_position.y + extent.height);
      }

      [[nodiscard]] constexpr auto is_inside(rect const& other) const noexcept -> bool
      {
         return is_inside(other.top_left_position) && other.width() <= extent.width && other.height() <= extent.height;
      }

      template <typename T>
      [[nodiscard]] constexpr auto operator/(T rhs) const noexcept
      {
         return rect{top_left_position, extent / rhs};
      }

      friend auto operator<<(std::ostream& os, rect const& obj) -> decltype(auto)
      {
         return os << obj.top_left_position << ' ' << obj.extent;
      }
   };

   [[nodiscard]] constexpr bool operator==(rect const& lhs, rect const& rhs) noexcept
   {
      return std::tie(lhs.top_left_position, lhs.extent) == std::tie(rhs.top_left_position, rhs.extent);
   }

   [[nodiscard]] inline auto to_string(rect const& obj)
   {
      std::stringstream result{};
      result << obj;
      return result.str();
   }

} // namespace orpg
