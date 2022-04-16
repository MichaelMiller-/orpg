#pragma once

#include "entities.h"
#include "tile_map.h"

namespace orpg
{
   // clang-format off
    //
    // 0 ----->  x
    // |
    // |
    // v
    //
    // y

    //! \todo generate from editor -> header with tile-map's
    static constexpr std::uint8_t tiles_house1[7][6] = {
            {0, 1, 2, 3, 4, 5},
            {0, 1, 2, 3, 4, 5},
            {0, 1, 2, 3, 4, 5},
            {0, 1, 2, 3, 4, 5},
            {0, 1, 2, 3, 4, 5},
            {0, 1, 2, 3, 4, 5},
            {0, 1, 2, 3, 4, 5},
    };
    static constexpr entities_t entities_house1[7][6] = {
            {none{}, portal{world1{}, {5, 16}}, none{},    none{}, none{}, none{}},
            {none{}, none{}, none{},    none{}, none{}, none{}},
            {none{}, none{}, none{},    none{}, none{}, portal{house2{}, {0, 2}}},
            {none{}, none{}, none{},    none{}, none{}, none{}},
            {none{}, none{}, blocked{}, none{}, none{}, none{}},
            {none{}, none{}, blocked{}, none{}, none{}, none{}},
            {none{}, none{}, blocked{}, none{}, none{}, none{}},
    };
    static constexpr std::uint8_t tiles_house2[4][4] = {
            {0, 1, 2, 100},
            {0, 1, 2, 3},
            {0, 1, 2, 3},
            {0, 1, 2, 3},
    };
    static constexpr entities_t entities_house2[4][4] = {
            {none{},                   portal{house3{}, {2, 30}}, chest{}, none{}},
            {none{},                   none{}, none{}, chest{}},
            {portal{house1{}, {5, 2}}, none{}, none{}, none{}},
            {none{},                   none{}, none{}, none{}},
    };
    static constexpr std::uint8_t tiles_house3[30][5] = {
            {0, 0, 100, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 2, 3, 2},
            {0, 1, 2, 3, 2},
            {0, 1, 2, 2, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 0, 0},
            {0, 1, 2, 0, 0},
            {0, 1, 2, 1, 4},
            {0, 1, 2, 1, 4},
            {0, 1, 2, 2, 0},
            {0, 1, 2, 2, 0},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
            {0, 1, 2, 3, 4},
    };
    static constexpr entities_t entities_house3[30][5] = {
            {none{}, none{}, chest{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, portal{house2{}, {1, 0}}, none{}, none{}},
    };

    static constexpr std::uint8_t tiles_world1[30][45] = {
            {2, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 2, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 2, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 2, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 2, 0, 2, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            // row 7
            {7, 0, 1, 0, 2, 0, 0, 2, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 2, 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            // row 14
            {3, 3, 1, 3, 2, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 1, 0, 2, 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {0, 0, 1, 0, 2, 0, 0, 2, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 1, 0, 2, 0, 2, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {0, 0, 1, 0, 2, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {0, 0, 1, 2, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
            {0, 2, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
            {2, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    };
    static constexpr entities_t entities_world1[30][45] = {
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, blocked{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, blocked{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, blocked{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, portal{house1{}, {1, 0}}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, blocked{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, blocked{}, blocked{}},
    };
    // 20x15 * 32x32 == 640x480
    static constexpr std::uint8_t tiles_test_world[15][20] = {
            {3, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
            {3, 2, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
            {3, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
            {3, 0, 1, 2, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
            {3, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
            {3, 0, 1, 0, 2, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
            {3, 0, 1, 0, 2, 0, 2, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
            {3, 0, 1, 0, 2, 0, 0, 2, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
            {3, 0, 1, 0, 2, 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
            {3, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 },
            {3, 0, 1, 0, 2, 0, 0, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 2, 3 },
            {3, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 2, 0, 0, 0, 0, 0, 2, 0, 3 },
            {3, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 2, 0, 0, 0, 2, 0, 0, 3 },
            {3, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 2, 0, 2, 0, 0, 0, 3 },
            {3, 0, 1, 0, 2, 0, 0, 0, 0, 4, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3 },
    };
    static constexpr entities_t entities_test_world[15][20] = {
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, chest{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
            {none{}, none{}, none{}, none{}, none{}, blocked{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}, none{}},
    };
    // clang-format on

    using test_world_t = tile_map<test_world, 15, 20, tiles_test_world, entities_test_world>;
    using village1_house1_t = tile_map<house1, 7, 6, tiles_house1, entities_house1>;
    using village1_house2_t = tile_map<house2, 4, 4, tiles_house2, entities_house2>;
    using village1_house3_t = tile_map<house3, 30, 5, tiles_house3, entities_house3>;
    using world1_t = tile_map<world1, 30, 45, tiles_world1, entities_world1>;

} // namespace orpg
