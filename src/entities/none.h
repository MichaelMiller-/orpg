#pragma once

#include <ostream>

namespace orpg
{
   struct none
   {
      friend auto& operator<<(std::ostream& os, none const&) { return os << "none"; }
   };
} // namespace orpg
