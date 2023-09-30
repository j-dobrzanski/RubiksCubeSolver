#ifndef CUBE_HPP
#define CUBE_HPP

#include "cube_tile.hpp"
#include <vector>

typedef std::vector<std::vector<CubeTile>> Side;

enum class Axis {
    X,
    Y,
    Z
};

class Cube{
    public:
        uint8_t size;
        std::vector<std::vector<CubeTile*>*>* side_front;
        std::vector<std::vector<CubeTile*>*>* side_back;
        std::vector<std::vector<CubeTile*>*>* side_left;
        std::vector<std::vector<CubeTile*>*>* side_right;
        std::vector<std::vector<CubeTile*>*>* side_top;
        std::vector<std::vector<CubeTile*>*>* side_bottom;

        Cube(uint8_t size){
            std::vector<int> super;
        }
        int rotate(Axis axis, uint8_t tile_index);
        int check_solution();

    private:
        std::vector<std::vector<CubeTile*>*>* initialize_side(side side);
        
};

#endif