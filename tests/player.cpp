#include <catch.hpp>

// #include "src/rect.h"

struct player
{
    bool connected{false};
    std::string name{};
};

template <auto N>
constexpr auto connected(std::array<player, N> const& a)
{
    return std::count_if(begin(a), end(a), [](auto const& e){ return e.connected; });
}


TEST_CASE("player management", "[orpg]")
{
    SECTION("initial is no player connected")
    {
        std::array<player, 4> players;
        REQUIRE(connected(players) == 0);
    }
    SECTION("connect a new player")
    {
        std::array<player, 4> players;
        players[0] = { true, "p1"};
        REQUIRE(connected(players) == 1);
    }
    SECTION("connect another player")
    {
        std::array<player, 4> players;
        players[0] = { true, "p1"};
        players[1] = { true, "p2"};
        REQUIRE(connected(players) == 2);
    }
    SECTION("disconnect first player")
    {
        std::array<player, 4> players;
        players[0] = { true, "p1"};
        players[1] = { true, "p2"};
        REQUIRE(connected(players) == 2);
        players[0].connected = false;
        REQUIRE(connected(players) == 1);
    }
}