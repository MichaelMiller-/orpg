#pragma once

#include "rect.h"

#include <string>
#include <array>

namespace orpg
{
   struct settings
   {
      static constexpr auto valid_window_resolutions = std::array{
         extents{640, 480},
         extents{1024, 768},
      };
      std::string window_title{};
      rect window{{0, 0}, {1024, 768}};
      bool fullscreen{false};
   };
}