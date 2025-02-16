#pragma once

#include <nlohmann/json.hpp>

#include <format>
#include <fstream>

namespace orpg
{
   template <typename T>
   [[nodiscard]] auto read_from_json(std::filesystem::path const& filename) noexcept -> T
   {
      T result;
      try {
         std::ifstream ifs{filename};
         auto j = nlohmann::json::parse(ifs);
         result = j.get<T>();
      } catch (...) {
         std::puts(std::format("failed to open and parse: {} \n -> use default", filename.string()).c_str());
      }
      return result;
   }
} // namespace orpg