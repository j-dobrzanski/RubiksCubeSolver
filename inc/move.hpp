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

        bool is_cube_rotation;

        std::string to_string();

        /**
         *  Used for whole cube rotations
         *  It should save sides Right/Left, Front/Back or Up/Down
         *  and display them as x, y, or z 
         *
         *  @param[in]  side    
         *  @param[in]  is_half_turn
         */
        Move(Side side, bool is_clockwise, bool is_half_turn);
        Move(Side side, uint8_t tile_index, bool clockwise, bool is_half_turn);
        Move(std::string str);
        Move(Move* move);
};

/**
 *  Checks if making new_move on the top of the lsat_move from cube solution path
 *  makes sense, e.g. whether it has a chance of prducing new cube configuration
 *
 *  @param[in]  last_move   last move on cube solution path (last move made on the cube)
 *  @param[in]  new_move    new move that is going to be performed on cube
 *
 *  @return     true if move makes sense, false if there is no point of making it
 */
bool check_move_sanity(Move* last_move, Move* new_move);

#endif
