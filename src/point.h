#pragma once

#include <cstdint>
#include <ostream>
#include <sstream>

namespace orpg
{
   struct point
   {
      using value_t = std::int32_t;

      value_t x{0};
      value_t y{0};

      [[nodiscard]] constexpr auto operator==(point const& other) const -> bool = default;

      [[nodiscard]] constexpr auto operator!=(point const& other) const -> bool = default;

      friend auto operator<<(std::ostream& os, point const& obj) -> decltype(auto)
      {
         return os << std::format("({},{})", obj.x, obj.y);
      }
   };

   [[nodiscard]] inline auto to_string(point const& obj)
   {
      std::stringstream result{};
      result << obj;
      return result.str();
   }

   [[nodiscard]] constexpr auto operator+(point const& lhs, point const& rhs) noexcept -> point
   {
      return {.x = lhs.x + rhs.x, .y = lhs.y + rhs.y};
   }

   [[nodiscard]] constexpr auto operator-(point const& lhs, point const& rhs) noexcept -> point
   {
      return {.x = lhs.x - rhs.x, .y = lhs.y - rhs.y};
   }

   [[nodiscard]] constexpr auto operator/(point const& lhs, std::integral auto const& rhs) noexcept -> point
   {
      return {.x = lhs.x / rhs, .y = lhs.y / rhs};
   }
} // namespace orpg
