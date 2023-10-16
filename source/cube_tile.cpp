#include "../inc/cube_tile.hpp"

char side_to_char(Side side){
    switch(side){
        case Side::Up:
            return 'U';
        case Side::Front:
            return 'F';
        case Side::Right:
            return 'R';
        case Side::Back:
            return 'B';
        case Side::Left:
            return 'L';
        case Side::Down:
            return 'D';
        default:
            return -1;
    }
}