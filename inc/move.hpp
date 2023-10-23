#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>
#include <string>

enum class Side {
    Front,
    Back,
    Left,
    Right,
    Up,
    Down,
    Error
};

char side_to_char(Side side);

class Move {
    public:
        Side side;
        uint8_t tile_index;
        bool clockwise;
        bool is_half_turn;

        std::string to_string();

        Move(Side side, uint8_t tile_index, bool clockwise, bool is_half_turn);
        Move(std::string str);
        Move(Move* move);
};

#endif
