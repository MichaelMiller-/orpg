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

#include <nlohmann/json.hpp>

namespace orpg
{
   inline void to_json(nlohmann::json& j,settings const& obj)
   {
      j["window_title"] = obj.window_title;
      j["window"] = obj.window;
      j["fullscreen"] = obj.fullscreen;
   }

   inline void from_json(nlohmann::json const& j,settings& obj)
   {
      j.at("window_title").get_to(obj.window_title);
      j.at("window").get_to(obj.window);
      j.at("fullscreen").get_to(obj.fullscreen);
   }
}