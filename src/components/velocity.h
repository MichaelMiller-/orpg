#pragma once

#include "position.h"

namespace orpg
{
   struct velocity
   {
      int dx;
      int dy;
   };
   auto operator+(position const& lhs, velocity const& rhs) noexcept -> position
   {
      return {lhs.x + rhs.dx, lhs.y + rhs.dy};
   }
} // namespace orpg