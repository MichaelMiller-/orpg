#pragma once

#include "extents.h"

namespace orpg
{
    template <typename Tag, auto Y, auto X, auto const& Tiles, auto const& Entities>
    struct tile_map {
        static_assert(X > 0);
        static_assert(Y > 0);

        using tag_t = Tag;

        static inline constexpr std::uint16_t rows_v = Y;
        static inline constexpr std::uint16_t cols_v = X;
        static inline constexpr std::uint16_t tile_size_v = 32;

        static constexpr decltype(Tiles) tiles = Tiles;
        static constexpr decltype(Entities) entities = Entities;
    };

    template <typename Tag, auto Y, auto X, auto const& Tiles, auto const& Entities>
    [[nodiscard]] constexpr auto extent(tile_map<Tag, Y, X, Tiles, Entities>) noexcept
    {
        return extents{X, Y};
    }
}