#pragma once

#include "../point.h"
#include "../tags.h"

namespace orpg
{
   struct portal
   {
      friend auto& operator<<(std::ostream& os, portal const&) { return os << "portal"; }

      //! \brief target map
      map_tags_t target{};
      //! \brief target player position in world coordinates
      point position{};
   };
} // namespace orpg
