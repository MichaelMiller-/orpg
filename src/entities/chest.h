#pragma once

namespace orpg
{
   struct chest
   {
      friend auto& operator<<(std::ostream& os, chest const&) { return os << "chest"; }
   };
} // namespace orpg
