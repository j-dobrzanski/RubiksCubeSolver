#ifndef CUBE_TILE_HPP
#define CUBE_TILE_HPP

#include <cstdint>

enum class side {
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom
};

enum class orientation {
    North,
    South,
    East,
    West
};

class CubeTile {
    public:
        uint8_t x_destination, y_destination;
        side side_current, side_destination;
        uint8_t orientation_current, orientation_destination;

        CubeTile(uint8_t x_destination,
                    uint8_t y_destination,
                    side side_current,
                    side side_destination,
                    orientation orientation_current,
                    orientation orientation_destination);
};

#endif