#pragma once

#include <filesystem>
#include <fstream>
#include <ostream>
#include <string>
#include <unordered_map>

namespace orpg
{
   using configuration_t = std::unordered_map<std::string, std::string>;

   inline auto operator>>(std::ifstream& file, configuration_t& obj) -> decltype(auto)
   {
      std::string line;
      while (std::getline(file, line)) {
         // Skip comments
         if (line.empty() || line[0] == '#') {
            continue;
         }

         // Split line into key and value based on '='
         auto delimiter = line.find('=');
         if (delimiter != std::string::npos) {
            auto key = line.substr(0, delimiter);
            auto value = line.substr(delimiter + 1);

            // Trim whitespace
            key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);
            value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);

            obj[key] = value;
         }
      }
      return file;
   }

   inline auto operator<<(std::ostream& output, configuration_t const& obj) -> decltype(auto)
   {
      for (auto const& [key, value] : obj) {
         output << key << " = " << value << '\n';
      }
      return output;
   }

   [[nodiscard]] inline auto read_configuration(std::filesystem::path const& filename) -> configuration_t
   {
      std::ifstream file{filename};
      configuration_t config;
      file >> config;
      file.close();
      return config;
   }
} // namespace orpg