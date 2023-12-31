#ifndef CUBE_HPP
#define CUBE_HPP

#include "move.hpp"
#include "cube_tile.hpp"
#include <cstdint>
#include <vector>

enum class CubeType {
    BASIC,
    PICTURE
};

class Cube {
    public:

        size_t size;
        std::vector<std::vector<CubeTile*>*>* side_front;
        std::vector<std::vector<CubeTile*>*>* side_back;
        std::vector<std::vector<CubeTile*>*>* side_left;
        std::vector<std::vector<CubeTile*>*>* side_right;
        std::vector<std::vector<CubeTile*>*>* side_up;
        std::vector<std::vector<CubeTile*>*>* side_down;

        int rotate();
        int check_solution();
};


#endif