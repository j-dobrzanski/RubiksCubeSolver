#ifndef PICTURE_CUBE_HPP
#define PICTURE_CUBE_HPP

#include "cube.hpp"
#include "picture_cube_tile.hpp"
#include <vector>

// typedef std::vector<std::vector<CubeTile>> Side;

class PictureCube : Cube{
    public:
        uint8_t size;
        CubeType type;
        std::vector<std::vector<PictureCubeTile*>*>* side_front;
        std::vector<std::vector<PictureCubeTile*>*>* side_back;
        std::vector<std::vector<PictureCubeTile*>*>* side_left;
        std::vector<std::vector<PictureCubeTile*>*>* side_right;
        std::vector<std::vector<PictureCubeTile*>*>* side_top;
        std::vector<std::vector<PictureCubeTile*>*>* side_bottom;

        PictureCube(uint8_t size, CubeType type);
        int rotate(Axis axis, uint8_t tile_index);
        int check_solution();

    private:
        std::vector<std::vector<PictureCubeTile*>*>* initialize_side(Side side);
        int check_solution_basic();
        int check_solution_picture();
        
};

#endif