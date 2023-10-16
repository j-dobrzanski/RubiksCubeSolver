#ifndef BASIC_CUBE_HPP
#define BASIC_CUBE_HPP

#include "cube.hpp"
#include "basic_cube_tile.hpp"
#include <cstdint>
#include <vector>

// typedef std::vector<std::vector<CubeTile>> Side;

/*
polygon mesh for 3x3x3:
                    [T1.1][T1.2][T1.3]
                    [T2.1][T2.2][T2.3]
                    [T3.1][T3.2][T3.3]

[L1.1][L1.2][L1.3]  [F1.1][F1.2][F1.3]  [R1.1][R1.2][R1.3]  [B1.1][B1.2][B1.3]
[L2.1][L2.2][L2.3]  [F2.1][F2.2][F2.3]  [R2.1][R2.2][R2.3]  [B2.1][B2.2][B2.3]
[L3.1][L3.2][L3.3]  [F3.1][F3.2][F3.3]  [R3.1][R3.2][R3.3]  [B3.1][B3.2][B3.3]

                    [B3.1][B3.2][B3.3]
                    [B2.1][B2.2][B2.3]
                    [B1.1][B1.2][B1.3]
*/

class BasicCube : Cube{
    public:
        // uint8_t size;
        std::vector<std::vector<BasicCubeTile*>*>* side_front;
        std::vector<std::vector<BasicCubeTile*>*>* side_back;
        std::vector<std::vector<BasicCubeTile*>*>* side_left;
        std::vector<std::vector<BasicCubeTile*>*>* side_right;
        std::vector<std::vector<BasicCubeTile*>*>* side_up;
        std::vector<std::vector<BasicCubeTile*>*>* side_down;

        BasicCube(uint8_t size);
        int rotate(Axis axis, uint8_t tile_index, bool clockwise);
        int check_solution();
        void print_cube();
        int rotate_left(uint8_t tile_index, bool clockwise);
        int rotate_right(uint8_t tile_index, bool clockwise);
        int rotate_front(uint8_t tile_index, bool clockwise);
        int rotate_back(uint8_t tile_index, bool clockwise);
        int rotate_up(uint8_t tile_index, bool clockwise);
        int rotate_down(uint8_t tile_index, bool clockwise);
        int compare_cube(BasicCube* cube);

        ~BasicCube();

    private:
        std::vector<std::vector<BasicCubeTile*>*>* initialize_side(Side side);
        int check_solution_basic();
        int check_solution_picture();
        
};

#endif