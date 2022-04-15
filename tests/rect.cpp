#include <catch.hpp>

#include "src/rect.h"

TEST_CASE("geometric rectangle class", "[geometry]")
{
    using namespace orpg;

    const rect rc1{ { 10, 5 }, { 40, 60 } };

    SECTION("access methods")
    {
        REQUIRE(rc1.left() == 10);
        REQUIRE(rc1.right() == 50);
        REQUIRE(rc1.top() == 5);
        REQUIRE(rc1.bottom() == 65);
        REQUIRE(rc1.width() == 40);
        REQUIRE(rc1.height() == 60);
    }
    SECTION("center rect")
    {
        const auto result = rc1.centered({ 100, 100 });

        REQUIRE(result.left() == 30);
        REQUIRE(result.right() == 70);
        REQUIRE(result.top() == 20);
        REQUIRE(result.bottom() == 80);
        REQUIRE(result.width() == 40);
        REQUIRE(result.height() == 60);
    }
    SECTION("intersection")
    {
        REQUIRE(rc1.intersects({ { 0, 0 }, { 5, 5 } }) == false);
        REQUIRE(rc1.intersects({ { 0, 0 }, { 15, 15 } }) == true);
    }
    SECTION("calculate center point")
    {
        REQUIRE(rc1.center() == point{30, 35});
    }
    SECTION("check if a point is inside the rect")
    {
        REQUIRE(rc1.is_inside(point{ 0, 0 }) == false);
        REQUIRE(rc1.is_inside(point{ 0, 5 }) == false);
        REQUIRE(rc1.is_inside(point{ 10, 5 }) == true);
        REQUIRE(rc1.is_inside(point{ 20, 30 }) == true);
        REQUIRE(rc1.is_inside(point{ 50, 35 }) == true);
        REQUIRE(rc1.is_inside(point{ 50, 65 }) == true);
        REQUIRE(rc1.is_inside(point{ 100, 5 }) == false);
        REQUIRE(rc1.is_inside(point{ 100, 100 }) == false);
    }
    SECTION("check if another rect is full inside")
    {
        REQUIRE(rc1.is_inside({ { 0, 0 }, { 5, 5 } }) == false);
        REQUIRE(rc1.is_inside({ { 0, 0 }, { 15, 15 } }) == false);
        REQUIRE(rc1.is_inside({ { 10, 10 }, { 15, 15 } }) == true);
        REQUIRE(rc1.is_inside({ { 10, 10 }, { 40, 15 } }) == true);
        REQUIRE(rc1.is_inside({ { 40, 60 }, { 100, 100 } }) == false);
    }
    SECTION("divide")
    {
        const auto result = rc1 / 2;
        REQUIRE(result == rect{ {10, 5}, { 20, 30}});
    }
}