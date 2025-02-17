#include <catch2/catch.hpp>  // Use catch2 header (adjust if your project uses a different Catch2 version)
#include <array>
#include <algorithm>
#include <string>

// Represents a player with a connection state and a name.
struct Player {
    bool connected{false};
    std::string name{};
};

// Counts the number of connected players in the given array.
template <std::size_t N>
constexpr std::size_t countConnectedPlayers(const std::array<Player, N>& players) {
    return std::count_if(players.begin(), players.end(), [](const Player& p) {
        return p.connected;
    });
}

TEST_CASE("Player management", "[orpg]") {
    SECTION("Initial state: no player connected") {
        std::array<Player, 4> players;
        REQUIRE(countConnectedPlayers(players) == 0);
    }
    SECTION("Connect a new player") {
        std::array<Player, 4> players;
        players[0] = { true, "p1" };
        REQUIRE(countConnectedPlayers(players) == 1);
    }
    SECTION("Connect another player") {
        std::array<Player, 4> players;
        players[0] = { true, "p1" };
        players[1] = { true, "p2" };
        REQUIRE(countConnectedPlayers(players) == 2);
    }
    SECTION("Disconnect first player") {
        std::array<Player, 4> players;
        players[0] = { true, "p1" };
        players[1] = { true, "p2" };
        REQUIRE(countConnectedPlayers(players) == 2);
        players[0].connected = false;
        REQUIRE(countConnectedPlayers(players) == 1);
    }
}
