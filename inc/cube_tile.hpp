#ifndef CUBE_TILE_HPP
#define CUBE_TILE_HPP

enum class Side {
    Front,
    Back,
    Left,
    Right,
    Up,
    Down
};

enum class Orientation {
    North,
    South,
    East,
    West
};

class CubeTile{

};

char side_to_char(Side side);

#endif