#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <array>
#include <string>

// Represents a player with a connection state and a name.
struct player
{
   bool connected{false};
   std::string name{};
};

constexpr auto count_connected_players(std::ranges::range auto const& players)
{
   return std::ranges::count(players, true, &player::connected);
}

TEST_CASE("player management", "[orpg]")
{
   SECTION("initial is no player connected")
   {
      std::array<player, 4> players;
      REQUIRE(count_connected_players(players) == 0);
   }
   SECTION("connect a new player")
   {
      std::array<player, 4> players;
      players[0] = {true, "p1"};
      REQUIRE(count_connected_players(players) == 1);
   }
   SECTION("connect another player")
   {
      std::array<player, 4> players;
      players[0] = {true, "p1"};
      players[1] = {true, "p2"};
      REQUIRE(count_connected_players(players) == 2);
   }
   SECTION("disconnect first player")
   {
      std::array<player, 4> players;
      players[0] = {true, "p1"};
      players[1] = {true, "p2"};
      REQUIRE(count_connected_players(players) == 2);
      players[0].connected = false;
      REQUIRE(count_connected_players(players) == 1);
   }
}