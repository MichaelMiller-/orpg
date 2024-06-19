#include <catch.hpp>

#include "point.h"

TEST_CASE("point", "[geometry]")
{
   using namespace orpg;

   const point pt1{5, 5};
   const point pt2{2, 7};

   SECTION("compare")
   {
      REQUIRE(pt1 == point{5, 5});
      REQUIRE(pt1 != point{3, 5});
      REQUIRE(pt1 != point{5, 2});
   }
   SECTION("addition")
   {
      const auto result = pt1 + pt2;

      REQUIRE(result.x == 7);
      REQUIRE(result.y == 12);
   }
}