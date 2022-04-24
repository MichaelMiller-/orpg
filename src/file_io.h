#pragma once

#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>

#include <fstream>

namespace orpg
{
   template <typename T>
   [[nodiscard]] auto read_from_json(std::filesystem::path const& filename) noexcept -> T
   {
      T result;
      try {
         std::ifstream ifs{filename};
         const auto j = nlohmann::json::parse(ifs);
         result = j.get<T>();
      } catch (...) {
         spdlog::warn("failed to open and parse: {} \n -> use default", filename.string());
      }
      return result;
   }
} // namespace orpg