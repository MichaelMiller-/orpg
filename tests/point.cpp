#include <catch.hpp>

#include "src/point.h"

TEST_CASE("add to points", "[geometry]")
{
    using namespace orpg;

    const point pt1{5, 5};
    const point pt2{2, 7};

    const auto result = pt1 + pt2;

    REQUIRE(result.x == 7);
    REQUIRE(result.y == 12);
}