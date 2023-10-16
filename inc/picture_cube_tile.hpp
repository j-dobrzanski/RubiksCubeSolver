#ifndef PICTURE_CUBE_TILE_HPP
#define PICTURE_CUBE_TILE_HPP

#include "cube_tile.hpp"
#include <cstdint>

class PictureCubeTile : CubeTile {
    public:
        uint8_t x_destination, y_destination;
        Side side_current, side_destination;
        Orientation orientation_current, orientation_destination;

        PictureCubeTile(uint8_t x_destination,
                    uint8_t y_destination,
                    Side side_current,
                    Side side_destination,
                    Orientation orientation_current,
                    Orientation orientation_destination);
};

#endif