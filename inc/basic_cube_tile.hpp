#ifndef BASIC_CUBE_TILE_HPP
#define BASIC_CUBE_TILE_HPP

#include "cube_tile.hpp"
#include <cstdint>

class BasicCubeTile : CubeTile {
    public:
        Side side_destination;

        BasicCubeTile(Side side_destination);
};

#endif