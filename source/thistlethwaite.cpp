#include "../inc/thistlethwaite.hpp"
#include <array>
#include <cstdint>
#include <fstream>
#include <queue>
#include <tuple>
#include <utility>
#include <iostream>
#include <algorithm>
#include <vector>
#include <bitset>
#include <cmath>
#include <stack>
#include <iomanip>

static inline std::map<std::pair<Side,Side>, bool>* initialize_phase1_map();

static inline bool check_move_sanity(Move* last_move, Move* new_move);

static inline uint16_t corner_orientation_value(uint16_t number, Side clockwise, Side anticlockwise);
static inline uint16_t corner_orientations_to_id(uint16_t number);
static inline uint16_t edge_permutation_value(uint16_t number, Side side1, Side side2, std::array<std::pair<Side, Side>, 4>* edge_slice);
static inline uint16_t edge_permutation_slice_Y_to_id(uint16_t permutation_value);
static inline void print_corner_orientation(uint16_t number);

static inline bool corners_in_orbits(BasicCube* cube, std::array<std::tuple<Side, Side, Side>, 4> orbit);
static inline bool compare_corner_orbit_tuple(std::tuple<Side, Side, Side> tuple, std::array<std::tuple<Side, Side, Side>, 4> orbit);
static inline bool compare_tuple(std::tuple<Side, Side, Side> tuple1, std::tuple<Side, Side, Side> tuple2);
static inline bool compare_pair(std::pair<Side, Side> pair1, std::pair<Side, Side> pair2);
static inline uint16_t edge_permutation_slice_X_to_id(uint16_t permutation_value);

static inline uint32_t corner_permutation_to_id(uint32_t number);

#define PHASE_1_DEPTH 6
#define PHASE_2_DEPTH 6
#define PHASE_3_DEPTH 6
#define PHASE_4_DEPTH 6

uint16_t permutation_to_id_lookup_table_phase_2[4096] = {0};
bool permutation_to_id_lookup_table_initialized_phase_2 = false;

uint16_t permutation_to_id_lookup_table_phase_3[255] = {0};
bool permutation_to_id_lookup_table_initialized_phase_3 = false;

uint16_t permutation_to_id_lookup_table_phase_4[40320] = {0};
bool permutation_to_id_lookup_table_initialized_phase_4 = false;

uint16_t temp_debug_value = 0;

Thistlethwaite::Thistlethwaite(BasicCube* cube){
    Thistlethwaite::start_cube = cube;
    this->solved_cube = NULL;

    this->phase1_move_count = 0;
    this->phase2_move_count = 0;
    this->phase3_move_count = 0;
    this->phase4_move_count = 0;

    current_cubes = new std::priority_queue<BasicCube*, std::vector<BasicCube*>, Compare_cubes>();
    current_cubes->push(new BasicCube(cube));

    Move* move = new Move(Side::Left, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);

    move = new Move(Side::Left, 0, false, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);

    move = new Move(Side::Left, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);


    move = new Move(Side::Right, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);

    move = new Move(Side::Right, 0 , false, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);

    move = new Move(Side::Right, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);


    move = new Move(Side::Front, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);

    move = new Move(Side::Front, 0, false, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);

    move = new Move(Side::Front, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);


    move = new Move(Side::Back, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);

    move = new Move(Side::Back, 0, false, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);

    move = new Move(Side::Back, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);


    move = new Move(Side::Up, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);


    move = new Move(Side::Up, 0, false, false);
    Thistlethwaite::moves_p1.push_back(move);


    move = new Move(Side::Up, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);


    move = new Move(Side::Down, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);

    move = new Move(Side::Up, 0, false, false);
    Thistlethwaite::moves_p1.push_back(move);

    move = new Move(Side::Down, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);

    Thistlethwaite::edge_orientation_map = initialize_phase1_map();

    this->generate_table_phase_1();
    this->generate_table_phase_2();
    this->generate_table_phase_3();
    this->generate_table_phase_4();

}

uint16_t Thistlethwaite::phase1_check(BasicCube* cube){
    uint16_t number = 0;


    // check ul
    Side side1 = (*cube->side_up)[1][0].side_destination;
    Side side2 = (*cube->side_left)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 9" << std::endl;
        number++;
    }
    number = number << 1;

    // check dl
    side1 = (*cube->side_down)[1][0].side_destination;
    side2 = (*cube->side_left)[CUBE_SIZE-1][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 11" << std::endl;
        number++;
    }
    number = number << 1;

    // check dr
    side1 = (*cube->side_down)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_right)[CUBE_SIZE-1][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 12" << std::endl;
        number++;
    }
    number = number << 1;

    // check ur
    side1 = (*cube->side_up)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_right)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 10" << std::endl;
        number++;
    }
    number = number << 1;

    // check bl
    side1 = (*cube->side_back)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_left)[1][0].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 8" << std::endl;
        number++;
    }
    number = number << 1;

    // check fl
    side1 = (*cube->side_front)[1][0].side_destination;
    side2 = (*cube->side_left)[1][CUBE_SIZE-1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 4" << std::endl;
        number++;
    }
    number = number << 1;

    // check fr
    side1 = (*cube->side_front)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_right)[1][0].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 2" << std::endl;
        number++;
    }
    number = number << 1;

    // check br
    side1 = (*cube->side_back)[1][0].side_destination;
    side2 = (*cube->side_right)[1][CUBE_SIZE-1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 6" << std::endl;
        number++;
    }
    number = number << 1;

    // check fu
    side1 = (*cube->side_front)[0][1].side_destination;
    side2 = (*cube->side_up)[CUBE_SIZE-1][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 1" << std::endl;
        number++;
    }
    number = number << 1;

    // check fd
    side1 = (*cube->side_front)[CUBE_SIZE-1][1].side_destination;
    side2 = (*cube->side_down)[CUBE_SIZE-1][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 3" << std::endl;
        number++;
    }
    number = number << 1;

    // check bd
    side1 = (*cube->side_back)[CUBE_SIZE-1][1].side_destination;
    side2 = (*cube->side_down)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 7" << std::endl;
        number++;
    }
    number = number << 1;

    // check bu
    side1 = (*cube->side_back)[0][1].side_destination;
    side2 = (*cube->side_up)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 5" << std::endl;
        number++;
    }
    number = number << 1;

    return number;
}

uint16_t Thistlethwaite::phase2_check(BasicCube* cube){
    // uint16_t number = 0;

    // Side side0 = (*cube->side_left)[0][0].side_destination;
    // number += (side0 != Side::Left && side0 != Side::Right);
    // side0 = (*cube->side_left)[0][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Left && side0 != Side::Right);
    // side0 = (*cube->side_left)[CUBE_SIZE-1][0].side_destination;
    // number += (side0 != Side::Left && side0 != Side::Right);
    // side0 = (*cube->side_left)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Left && side0 != Side::Right);

    // side0 = (*cube->side_right)[0][0].side_destination;
    // number += (side0 != Side::Left && side0 != Side::Right);
    // side0 = (*cube->side_right)[0][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Left && side0 != Side::Right);
    // side0 = (*cube->side_right)[CUBE_SIZE-1][0].side_destination;
    // number += (side0 != Side::Left && side0 != Side::Right);
    // side0 = (*cube->side_right)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Left && side0 != Side::Right);

    // Side side1 = (*cube->side_up)[CUBE_SIZE-1][1].side_destination;
    // Side side2 = (*cube->side_front)[0][1].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side1, side2)) == this->edge_slice_Y.end() &&
    //         std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side2, side1)) == this->edge_slice_Y.end()){
    //     number++;
    //     // std::cout << "case 1" << std::endl;
    // }

    // side1 = (*cube->side_front)[CUBE_SIZE-1][1].side_destination;
    // side2 = (*cube->side_down)[0][1].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side1, side2)) == this->edge_slice_Y.end() &&
    //         std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side2, side1)) == this->edge_slice_Y.end()){
    //     number++;
    //     // std::cout << "case 2" << std::endl;
    // }

    // side1 = (*cube->side_down)[CUBE_SIZE-1][1].side_destination;
    // side2 = (*cube->side_back)[CUBE_SIZE-1][1].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side1, side2)) == this->edge_slice_Y.end() &&
    //         std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side2, side1)) == this->edge_slice_Y.end()){
    //     number++;
    //     // std::cout << "case 3" << std::endl;
    // }

    // side1 = (*cube->side_back)[0][1].side_destination;
    // side2 = (*cube->side_up)[0][1].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side1, side2)) == this->edge_slice_Y.end() &&
    //         std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side2, side1)) == this->edge_slice_Y.end()){
    //     number++;
    //     // std::cout << "case 4" << std::endl;
    // }

    // return number;

    uint16_t index_corners = corner_orientations_to_id(
                                this->corner_orientations(cube));
    uint16_t index_edges = edge_permutation_slice_Y_to_id(
                                this->edge_slice_permutation(cube, &this->edge_slice_Y));
    return phase2_table[index_edges][index_corners] != 0;
}

uint16_t Thistlethwaite::phase3_check(BasicCube* cube){
    // uint16_t number = this->phase2_check(cube);

    // Side side0 = (*cube->side_front)[0][0].side_destination;
    // number += (side0 != Side::Front && side0 != Side::Back);
    // side0 = (*cube->side_front)[0][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Front && side0 != Side::Back);
    // side0 = (*cube->side_front)[CUBE_SIZE-1][0].side_destination;
    // number += (side0 != Side::Front && side0 != Side::Back);
    // side0 = (*cube->side_front)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Front && side0 != Side::Back);

    // side0 = (*cube->side_back)[0][0].side_destination;
    // number += (side0 != Side::Front && side0 != Side::Back);
    // side0 = (*cube->side_back)[0][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Front && side0 != Side::Back);
    // side0 = (*cube->side_back)[CUBE_SIZE-1][0].side_destination;
    // number += (side0 != Side::Front && side0 != Side::Back);
    // side0 = (*cube->side_back)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Front && side0 != Side::Back);

    // Side side1 = (*cube->side_left)[1][CUBE_SIZE-1].side_destination;
    // Side side2 = (*cube->side_front)[1][0].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side1, side2)) == this->edge_slice_X.end() &&
    //         std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side2, side1)) == this->edge_slice_X.end()){
    //     number++;
    //     // std::cout << "case 1" << std::endl;
    // }

    // side1 = (*cube->side_front)[1][CUBE_SIZE-1].side_destination;
    // side2 = (*cube->side_right)[1][0].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side1, side2)) == this->edge_slice_X.end() &&
    //         std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side2, side1)) == this->edge_slice_X.end()){
    //     number++;
    //     // std::cout << "case 2" << std::endl;
    // }

    // side1 = (*cube->side_right)[1][CUBE_SIZE-1].side_destination;
    // side2 = (*cube->side_back)[1][CUBE_SIZE-1].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side1, side2)) == this->edge_slice_X.end() &&
    //         std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side2, side1)) == this->edge_slice_X.end()){
    //     number++;
    //     // std::cout << "case 3" << std::endl;
    // }

    // side1 = (*cube->side_back)[1][0].side_destination;
    // side2 = (*cube->side_left)[1][0].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side1, side2)) == this->edge_slice_X.end() &&
    //         std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side2, side1)) == this->edge_slice_X.end()){
    //     number++;
    //     // std::cout << "case 4" << std::endl;
    // }


    // side0 = (*cube->side_up)[0][0].side_destination;
    // number += (side0 != Side::Up && side0 != Side::Down);
    // side0 = (*cube->side_up)[0][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Up && side0 != Side::Down);
    // side0 = (*cube->side_up)[CUBE_SIZE-1][0].side_destination;
    // number += (side0 != Side::Up && side0 != Side::Down);
    // side0 = (*cube->side_up)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Up && side0 != Side::Down);

    // side0 = (*cube->side_down)[0][0].side_destination;
    // number += (side0 != Side::Up && side0 != Side::Down);
    // side0 = (*cube->side_down)[0][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Up && side0 != Side::Down);
    // side0 = (*cube->side_down)[CUBE_SIZE-1][0].side_destination;
    // number += (side0 != Side::Up && side0 != Side::Down);
    // side0 = (*cube->side_down)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    // number += (side0 != Side::Up && side0 != Side::Down);

    // side1 = (*cube->side_up)[1][CUBE_SIZE-1].side_destination;
    // side2 = (*cube->side_right)[0][1].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side1, side2)) == this->edge_slice_Z.end() &&
    //         std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side2, side1)) == this->edge_slice_Z.end()){
    //     number++;
    //     // std::cout << "case 1" << std::endl;
    // }

    // side1 = (*cube->side_right)[CUBE_SIZE-1][1].side_destination;
    // side2 = (*cube->side_down)[1][CUBE_SIZE-1].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side1, side2)) == this->edge_slice_Z.end() &&
    //         std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side2, side1)) == this->edge_slice_Z.end()){
    //     number++;
    //     // std::cout << "case 2" << std::endl;
    // }

    // side1 = (*cube->side_down)[1][0].side_destination;
    // side2 = (*cube->side_left)[CUBE_SIZE-1][1].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side1, side2)) == this->edge_slice_Z.end() &&
    //         std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side2, side1)) == this->edge_slice_Z.end()){
    //     number++;
    //     // std::cout << "case 3" << std::endl;
    // }

    // side1 = (*cube->side_left)[0][1].side_destination;
    // side2 = (*cube->side_up)[1][0].side_destination;
    // // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    // if(std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side1, side2)) == this->edge_slice_Z.end() &&
    //         std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side2, side1)) == this->edge_slice_Z.end()){
    //     number++;
    //     // std::cout << "case 4" << std::endl;
    // }

    // return number;


    uint32_t id_c = corner_permutation_to_id(this->corner_permutation(cube));
    uint16_t id_e = edge_permutation_slice_X_to_id(this->edge_slice_permutation(cube, &this->edge_slice_X));
    return phase3_table[id_c][id_e] != 0;
}

uint16_t Thistlethwaite::phase4_check(BasicCube* cube){
    return cube->check_solution() != 0;
}

Thistlethwaite::~Thistlethwaite(){
    while(!current_cubes->empty()){
        BasicCube* cube = current_cubes->top();
        current_cubes->pop();
        delete cube;
    }
    delete current_cubes;
    
    for(auto move : moves_p1){
        if(move != NULL){
            delete move;            
        }

    }

    for(auto cube : phase_3_table_initial_cubes){
        delete cube;
    }
    
    if(solved_cube != NULL){
        delete solved_cube;        
    }

    delete edge_orientation_map;
}

uint16_t Thistlethwaite::corner_orientations(BasicCube* cube){
    uint16_t number = 0;

    // check left-up corner on left side (L-B-U)
    number = corner_orientation_value(number,
                                      (*cube->side_up)[0][0].side_destination,
                                      (*cube->side_back)[0][0].side_destination);

    // check right-down corner on left side (L-F-D)
    number = corner_orientation_value(number,
                                      (*cube->side_down)[CUBE_SIZE - 1][0].side_destination,
                                      (*cube->side_front)[CUBE_SIZE - 1][0].side_destination);

    // check right-down corner on right side (R-B-D)
    number = corner_orientation_value(number,
                                      (*cube->side_down)[0][CUBE_SIZE - 1].side_destination,
                                      (*cube->side_back)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination); // suspicious

    // check left-up corner on right side (R-F-U)
    number = corner_orientation_value(number,
                                      (*cube->side_up)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination,
                                      (*cube->side_front)[0][CUBE_SIZE - 1].side_destination);

    // check right-up corner on left side (L-F-U)
    number = corner_orientation_value(number,
                                      (*cube->side_front)[0][0].side_destination,
                                      (*cube->side_up)[CUBE_SIZE - 1][0].side_destination);

    // check left-down corner on left side (L-B-D)
    number = corner_orientation_value(number,
                                      (*cube->side_back)[CUBE_SIZE - 1][0].side_destination,
                                      (*cube->side_down)[0][0].side_destination);

    // check left-down corner on right side (R-F-D)
    number = corner_orientation_value(number,
                                      (*cube->side_front)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination,
                                      (*cube->side_down)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination);

    // check right-up corner on right side (R-B-U)
    number = corner_orientation_value(number,
                                      (*cube->side_back)[0][CUBE_SIZE - 1].side_destination,
                                      (*cube->side_up)[0][CUBE_SIZE - 1].side_destination);
    
    return number;
}

uint16_t Thistlethwaite::edge_slice_permutation(BasicCube* cube, std::array<std::pair<Side, Side>, 4>* edge_slice){
    uint16_t number = 0;

    // slice Z ///////////////

    // check left-up edge
    number = edge_permutation_value(number,
                                    (*cube->side_left)[0][1].side_destination,
                                    (*cube->side_up)[1][0].side_destination,
                                    edge_slice);

    // check down-left edge
    number = edge_permutation_value(number,
                                    (*cube->side_down)[1][0].side_destination,
                                    (*cube->side_left)[CUBE_SIZE - 1][1].side_destination,
                                    edge_slice);

    // check right-down edge
    number = edge_permutation_value(number,
                                    (*cube->side_right)[CUBE_SIZE - 1][1].side_destination,
                                    (*cube->side_down)[1][CUBE_SIZE - 1].side_destination,
                                    edge_slice);

    // check up-right edge
    number = edge_permutation_value(number,
                                    (*cube->side_right)[0][1].side_destination,
                                    (*cube->side_up)[1][CUBE_SIZE - 1].side_destination,
                                    edge_slice);

    // slice X ///////////////

    // check left-back edge
    number = edge_permutation_value(number,
                                    (*cube->side_back)[1][0].side_destination,
                                    (*cube->side_left)[1][0].side_destination,
                                    edge_slice);

    // check left-front edge
    number = edge_permutation_value(number,
                                    (*cube->side_front)[1][0].side_destination,
                                    (*cube->side_left)[1][CUBE_SIZE - 1].side_destination,
                                    edge_slice);

    // check right-front edge
    number = edge_permutation_value(number,
                                    (*cube->side_front)[1][CUBE_SIZE - 1].side_destination,
                                    (*cube->side_right)[1][0].side_destination,
                                    edge_slice);

    // check right-back edge
    number = edge_permutation_value(number,
                                    (*cube->side_back)[1][CUBE_SIZE - 1].side_destination,
                                    (*cube->side_right)[1][CUBE_SIZE - 1].side_destination,
                                    edge_slice);

    // slice Y ///////////////

    // check up-front edge
    number = edge_permutation_value(number,
                                    (*cube->side_front)[0][1].side_destination,
                                    (*cube->side_up)[CUBE_SIZE - 1][1].side_destination,
                                    edge_slice);

    // check down-front edge
    number = edge_permutation_value(number,
                                    (*cube->side_front)[CUBE_SIZE-1][1].side_destination,
                                    (*cube->side_down)[CUBE_SIZE - 1][1].side_destination,
                                    edge_slice);

    // check down-back edge
    number = edge_permutation_value(number,
                                    (*cube->side_back)[CUBE_SIZE-1][1].side_destination,
                                    (*cube->side_down)[0][1].side_destination,
                                    edge_slice);


    // check up-back edge
    number = edge_permutation_value(number,
                                    (*cube->side_back)[0][1].side_destination,
                                    (*cube->side_up)[0][1].side_destination,
                                    edge_slice);

    return number ^ 0xFFF;
}

int Thistlethwaite::clear_current_cubes(){
    while(!this->current_cubes->empty()){
        BasicCube* cube = this->current_cubes->top();
        this->current_cubes->pop();
        delete cube;
    }
    return 0;
}

int Thistlethwaite::bfs(std::vector<Move*>* allowed_moves,
                        uint16_t (Thistlethwaite::*check_function)(BasicCube*),
                        uint16_t (Thistlethwaite::*get_table_offset)(BasicCube*),
                        uint16_t max_depth){

    if(this->current_cubes->empty()){
        std::cout << "ERROR: bfs start with empty current_cubes" << std::endl;
        return -1;
    }

    BasicCube* base_cube = this->current_cubes->top();
    current_cubes->pop();
    if(!current_cubes->empty()){
        std::cout << "current cubes had more thatn 1 cube at start, that's wrong for now" << std::endl;
    }

    if(!(this->*check_function)(base_cube)){
        this->clear_current_cubes();
        this->current_cubes->push(base_cube);
        return 0;
    }


    uint64_t current_depth = base_cube->solution_length;

    for(uint16_t current_max_depth = 1; current_max_depth <= max_depth; current_max_depth++){
        this->current_cubes->push(new BasicCube(base_cube));
        while(!this->current_cubes->empty()){
            BasicCube* cube = this->current_cubes->top();
            this->current_cubes->pop();

            if(cube->solution_length > current_depth){
                current_depth = cube->solution_length;
                std::cout << "Current depth: " << current_depth << std::endl;
                // std::cout << "Something went wrong" << std::endl;
                // delete cube;
                // return -1;
            }
            for(auto move : *allowed_moves){
                if(cube->solution_length > 0 &&
                    !check_move_sanity(cube->solution_path->back(), move)){
                    continue;
                }
                // std::cout << move->to_string() << std::endl;
                BasicCube* new_cube = new BasicCube(cube);
                new_cube->rotate(move);

                // if(new_cube->temp_length > max_depth){
                if(new_cube->temp_length + (this->*get_table_offset)(new_cube) > current_max_depth){
                    // std::cout << "rejected" << std::endl;
                    // if(!(this->*check_function)(new_cube)){
                    //     new_cube->print_cube();
                    //     std::cout << "ERROR: Good cube rejected from heristics!!" << std::endl;
                    //     std::cout << "move count: " << new_cube->temp_length << ", ";
                    //     std::cout << "heurists for new cube: " << (this->*get_table_offset)(new_cube) << ", ";
                    //     std::cout << "heurists for old cube: " << (this->*get_table_offset)(cube) << ", ";
                    //     new_cube->print_solution_path();
                    //     std::cout << "max depth " << current_max_depth << std::endl;
                    //     delete new_cube;
                    //     delete cube;
                    //     return -1;
                    // }
                    temp_debug_value++;
                    delete new_cube;
                    continue;
                }
                new_cube->temp_length_plus_heuristic = new_cube->temp_length + (this->*get_table_offset)(new_cube);
                // new_cube->print_cube();
                // std::cout << (this->*check_function)(new_cube) << std::endl;
                if(!(this->*get_table_offset)(new_cube)){
                    this->clear_current_cubes();
                    this->current_cubes->push(new_cube);
                    delete cube;
                    delete base_cube;
                    return 0;
                }
                if(new_cube->temp_length < current_max_depth){
                    this->current_cubes->push(new_cube);
                }
                else{
                    delete new_cube;
                }
            }
            delete cube;
        }
    }

    delete base_cube;

    std::cout << "ERROR: bfs end with empty current_cubes" << std::endl;
    return 0;
}

int Thistlethwaite::generate_table_phase_1(){
    std::ifstream file(PHASE_1_TABLE_DIR_NAME);
    if(file.is_open()){// 'cached' table found
        std::cout << "Retreiving cached table for phase 1" << std::endl;
        int value;
        for(size_t id = 0; id < 2048; id++){
            file >> value;
            phase1_table[id] = (uint8_t)value;
        }
    }
    else{ // 'cached' table not found
        // need to generate table
        std::cout << "Generating & cacheing table for phase 1" << std::endl;
        BasicCube* cube = new BasicCube();
        std::stack<BasicCube*> cubes = std::stack<BasicCube*>();
        cubes.push(cube);

        // cubes that are not achievable in 8 moves
        for(size_t id = 0; id < 2048; id++){
            phase1_table[id] = PHASE_1_DEPTH + 1;
        }
        phase1_table[0] = 0;

        // all cubes that are achievable in up to 8 moves
        while(!cubes.empty()){
            cube = cubes.top();
            cubes.pop();

            for(auto move : this->moves_p1){

                if(cube->solution_length > 0 &&
                    !check_move_sanity(cube->solution_path->back(), move)){
                    continue;
                }

                BasicCube* child = new BasicCube(cube);
                child->rotate(move);
                
                uint16_t index = this->edge_orientation(child);

                if(phase1_table[index] > child->temp_length){
                    phase1_table[index] = child->temp_length;
                }
                // child->print_solution_path();
                // std::cout << index_edges << ", " << index_corners << std::endl;
                if(child->temp_length < PHASE_1_DEPTH){
                    cubes.push(child);
                }
                else{
                    delete child;
                }
            }
            delete cube;
        }

        // need to save table to file
        std::ofstream file(PHASE_1_TABLE_DIR_NAME);
        for(size_t id = 0; id < 2048; id++){
            file << (int)phase1_table[id] << std::endl;
        }
        std::cout << "Table 1 generted & cached" << std::endl;
    }
    return 0;
}

int Thistlethwaite::generate_table_phase_2(){
    std::ifstream file(PHASE_2_TABLE_DIR_NAME);
    if(file.is_open()){// 'cached' table found
        std::cout << "Retreiving cached table for phase 2" << std::endl;
        int value;
        for(size_t id_c = 0; id_c < 495; id_c++){
            for(size_t id_e = 0; id_e < 2187; id_e++){
                file >> value;
                phase2_table[id_c][id_e] = (uint8_t)value;
            }
        }
    }
    else{ // 'cached' table not found
        // need to generate table
        std::cout << "Generating & cacheing table for phase 2" << std::endl;
        BasicCube* cube = new BasicCube();
        std::stack<BasicCube*> cubes = std::stack<BasicCube*>();
        cubes.push(cube);

        // cubes that are not achievable in 8 moves
        for(size_t id_c = 0; id_c < 495; id_c++){
            for(size_t id_e = 0; id_e < 2187; id_e++){
                phase2_table[id_c][id_e] = PHASE_2_DEPTH + 1;
            }
        }
        phase2_table[0][0] = 0;

        // all cubes that are achievable in up to 8 moves
        while(!cubes.empty()){
            cube = cubes.top();
            cubes.pop();

            for(auto move : this->moves_p2){

                if(cube->solution_length > 0 &&
                    !check_move_sanity(cube->solution_path->back(), move)){
                    continue;
                }

                BasicCube* child = new BasicCube(cube);
                child->rotate(move);
                
                uint16_t index_corners = corner_orientations_to_id(
                                         this->corner_orientations(child));
                uint16_t index_edges = edge_permutation_slice_Y_to_id(
                                         this->edge_slice_permutation(child, &this->edge_slice_Y));
                
                if(phase2_table[index_edges][index_corners] > child->temp_length){
                    phase2_table[index_edges][index_corners] = child->temp_length;
                }
                // child->print_solution_path();
                // std::cout << index_edges << ", " << index_corners << std::endl;
                if(child->temp_length < PHASE_2_DEPTH){
                    cubes.push(child);
                }
                else{
                    delete child;
                }
            }
            delete cube;
            // if(cubes.size() > max_cubes){
            //     max_cubes = cubes.size();
            //     std::cout << "Current max number of cubes: " << max_cubes << std::endl;
            // }
            // std::cout << std::setw(10) << std::setfill('0') << counter << std::endl;
        }

        // size_t map[10] = {0};

        // need to save table to file
        std::ofstream file(PHASE_2_TABLE_DIR_NAME);
        for(size_t id_c = 0; id_c < 495; id_c++){
            for(size_t id_e = 0; id_e < 2187; id_e++){
                file << (int)phase2_table[id_c][id_e] << std::endl;
                // map[phase2_table[id_c][id_e]]++;
            }
        }
        std::cout << "Table 2 generted & cached" << std::endl;
        // for(size_t i = 0; i < 10; i++){
        //     std::cout << i << ": " << map[i] << std::endl;
        // }
    }
    return 0;
}

int Thistlethwaite::generate_table_phase_3(){
    std::ifstream file(PHASE_3_TABLE_DIR_NAME);
    if(file.is_open()){// 'cached' table found
        std::cout << "Retreiving cached table for phase 3" << std::endl;
        int value;
        for(size_t id_c = 0; id_c < 40320; id_c++){
            for(size_t id_e = 0; id_e < 70; id_e++){
                file >> value;
                phase3_table[id_c][id_e] = (uint8_t)value;
                // if(value != PHASE_3_DEPTH + 1){
                //     std::cout << "Reading: " << value << std::endl;
                // }
            }
        }
    }
    else{ // 'cached' table not found
        // need to generate table
        std::cout << "Generating & cacheing table for phase 3" << std::endl;
        this->initialize_phase_3_initial_cubes();

        std::stack<BasicCube*> cubes = std::stack<BasicCube*>();
        for(BasicCube* cube : this->phase_3_table_initial_cubes){
            BasicCube* temp = new BasicCube(cube);
            cubes.push(temp);
        }

        for(size_t id_c = 0; id_c < 40320; id_c++){
            for(size_t id_e = 0; id_e < 70; id_e++){
                if(phase3_table[id_c][id_e] == 0){
                    phase3_table[id_c][id_e] = PHASE_3_DEPTH + 1;
                }
                // else{
                //     std::cout << "Wirintg: " << (int)phase3_table[id_c][id_e] << std::endl;
                // }
            }
        }

        while(!cubes.empty()){
            BasicCube* cube = cubes.top();
            cubes.pop();

            for(auto move: this->moves_p3){

                if(cube->solution_length > 0 &&
                    !check_move_sanity(cube->solution_path->back(), move)){
                    continue;
                }

                BasicCube* child = new BasicCube(cube);
                child->rotate(move);

                uint32_t id_c = corner_permutation_to_id(this->corner_permutation(child));
                uint16_t id_e = edge_permutation_slice_X_to_id(this->edge_slice_permutation(child, &this->edge_slice_X));
                // std::cout << this->corner_permutation(child) << std::endl;
                // std::cout << id_c << ", " << id_e << std::endl;
                if(phase3_table[id_c][id_e] > child->temp_length){
                    phase3_table[id_c][id_e] = child->temp_length;
                    // std::cout << "wrote to table: " << child->temp_length << std::endl;
                }

                if(child->temp_length < PHASE_3_DEPTH){
                    cubes.push(child);
                }
                else{
                    delete child;
                }

            }

            delete cube;
        }

        phase3_table[0][0] = 0;

        // need to save table to file
        std::ofstream file(PHASE_3_TABLE_DIR_NAME);
        for(size_t id_c = 0; id_c < 40320; id_c++){
            for(size_t id_e = 0; id_e < 70; id_e++){
                file << (int)phase3_table[id_c][id_e] << std::endl;
            }
        }
        std::cout << "Table 3 generted & cached" << std::endl;

    }
    return 0;
}

int Thistlethwaite::generate_table_phase_4(){
    std::ifstream file(PHASE_4_TABLE_DIR_NAME);
    if(file.is_open()){// 'cached' table found
        std::cout << "Retrieving cached table for phase 4" << std::endl;
        int value;
        for(size_t id_c = 0; id_c < 96; id_c++){
            for(size_t id_e_slice_x = 0; id_e_slice_x < 24; id_e_slice_x++){
                for(size_t id_e_slice_y = 0; id_e_slice_y < 24; id_e_slice_y++){
                    for(size_t id_e_slice_z = 0; id_e_slice_z < 24; id_e_slice_z++){
                        file >> value;
                        phase4_table[id_c][id_e_slice_x][id_e_slice_y][id_e_slice_z] = (uint8_t)value;
                        // if(value != PHASE_4_DEPTH + 1){
                        //     std::cout << "Reading: " << value << std::endl;
                        // }
                    }
                }
            }
        }
    }
    else{ // 'cached' table not found
        // need to generate table
        std::cout << "Generating & cacheing table for phase 4" << std::endl;

        std::stack<BasicCube*> cubes = std::stack<BasicCube*>();
        cubes.push(new BasicCube());

        for(size_t id_c = 0; id_c < 96; id_c++){
            for(size_t id_e_slice_x = 0; id_e_slice_x < 24; id_e_slice_x++){
                for(size_t id_e_slice_y = 0; id_e_slice_y < 24; id_e_slice_y++){
                    for(size_t id_e_slice_z = 0; id_e_slice_z < 24; id_e_slice_z++){
                        phase4_table[id_c][id_e_slice_x][id_e_slice_y][id_e_slice_z] = PHASE_4_DEPTH + 1;
                    }
                }
            }
        }

        while(!cubes.empty()){
            BasicCube* cube = cubes.top();
            cubes.pop();

            for(auto move: this->moves_p4){

                if(cube->solution_length > 0 &&
                    !check_move_sanity(cube->solution_path->back(), move)){
                    continue;
                }

                BasicCube* child = new BasicCube(cube);
                child->rotate(move);


                uint16_t id_c = this->corner_permutation_to_id_phase4(corner_permutation_to_id(this->corner_permutation(child)));
                uint16_t id_e_slice_x = this->edge_slice_permutation_to_id(child, this->edge_slice_X);
                uint16_t id_e_slice_y = this->edge_slice_permutation_to_id(child, this->edge_slice_Y);
                uint16_t id_e_slice_z = this->edge_slice_permutation_to_id(child, this->edge_slice_Z);

                if(phase4_table[id_c][id_e_slice_x][id_e_slice_y][id_e_slice_z] > child->temp_length){
                    phase4_table[id_c][id_e_slice_x][id_e_slice_y][id_e_slice_z] = child->temp_length;
                }
                // std::cout << "move: " << move->to_string() << ", ";
                // std::cout << "id_c: " << id_c << ", ";
                // std::cout << "id_e_slice_y: " << id_e_slice_y << ", ";
                // std::cout << "id_e_slice_x: " << id_e_slice_x << ", ";
                // std::cout << "id_e_slice_z: " << id_e_slice_z << std::endl;

                if(child->temp_length < PHASE_4_DEPTH){ // should be 15
                    cubes.push(child);
                }
                else{
                    delete child;
                }

            }

            delete cube;
        }

        phase4_table[24][0][0][0] = 0;

        // need to save table to file
        std::ofstream file(PHASE_4_TABLE_DIR_NAME);
        for(size_t id_c = 0; id_c < 96; id_c++){
            for(size_t id_e_slice_x = 0; id_e_slice_x < 24; id_e_slice_x++){
                for(size_t id_e_slice_y = 0; id_e_slice_y < 24; id_e_slice_y++){
                    for(size_t id_e_slice_z = 0; id_e_slice_z < 24; id_e_slice_z++){
                        file << (int)phase4_table[id_c][id_e_slice_x][id_e_slice_y][id_e_slice_z] << std::endl;
                        // if(phase4_table[id_c][id_e_slice_x][id_e_slice_y][id_e_slice_z] != PHASE_4_DEPTH + 1){
                        //     std::cout << "Wrote: " << (int)phase4_table[id_c][id_e_slice_x][id_e_slice_y][id_e_slice_z] << std::endl;
                        // }
                    }
                }
            }
        }
        std::cout << "Table 4 generted & cached" << std::endl;

    }
    return 0;
}

int Thistlethwaite::solve(){
    this->bfs(&moves_p1,
              &Thistlethwaite::phase1_check,
              &Thistlethwaite::get_table_1_offset,
              PHASE_1_MAX_NO_OF_MOVES);
    std::cout << "End of phase 1" << std::endl;
    std::cout << "number of skipped cubes: " << temp_debug_value << std::endl;
    temp_debug_value = 0;
    std::cout << this->current_cubes->empty() << std::endl;
    if(!this->current_cubes->empty()){
        this->current_cubes->top()->temp_length=0;
        this->current_cubes->top()->print_cube();
        this->current_cubes->top()->print_solution_path();
        std::cout << this->phase1_check(this->current_cubes->top()) << std::endl;
        std::cout << "solution path length: " << this->current_cubes->top()->solution_length << " == " << this->current_cubes->top()->solution_path->size() << std::endl;
    }
    else{
        std::cout << "Error after phase 1" << std::endl;
        return 1;
    }

    this->bfs(&moves_p2,
              &Thistlethwaite::phase2_check,
              &Thistlethwaite::get_table_2_offset,
              PHASE_2_MAX_NO_OF_MOVES);
    std::cout << "End of phase 2" << std::endl;
    std::cout << "number of skipped cubes: " << temp_debug_value << std::endl;
    std::cout << this->current_cubes->empty() << std::endl;
    temp_debug_value = 0;
    if(!this->current_cubes->empty()){
        this->current_cubes->top()->temp_length=0;
        this->current_cubes->top()->print_cube();
        this->current_cubes->top()->print_solution_path();
        std::cout << this->phase2_check(this->current_cubes->top()) << std::endl;
        std::cout << "solution path length: " << this->current_cubes->top()->solution_length << " == " << this->current_cubes->top()->solution_path->size() << std::endl;
    }
    else{
        std::cout << "Error after phase 2" << std::endl;
        return 1;
    }

    this->bfs(&moves_p3,
              &Thistlethwaite::phase3_check,
              &Thistlethwaite::get_table_3_offset,
              PHASE_3_MAX_NO_OF_MOVES);
    std::cout << "End of phase 3" << std::endl;
    std::cout << "number of skipped cubes: " << temp_debug_value << std::endl;
    temp_debug_value = 0;
    std::cout << this->current_cubes->empty() << std::endl;
    if(!this->current_cubes->empty()){
        this->current_cubes->top()->temp_length=0;
        this->current_cubes->top()->print_cube();
        this->current_cubes->top()->print_solution_path();
        std::cout << this->phase3_check(this->current_cubes->top()) << std::endl;
        std::cout << "solution path length: " << this->current_cubes->top()->solution_length << " == " << this->current_cubes->top()->solution_path->size() << std::endl;
    }
    else{
        std::cout << "Error after phase 3" << std::endl;
        return 1;
    }
    this->bfs(&moves_p4,
             &Thistlethwaite::phase4_check,
             &Thistlethwaite::get_table_4_offset,
             PHASE_4_MAX_NO_OF_MOVES);
    std::cout << "End of phase 4" << std::endl;
    std::cout << "number of skipped cubes: " << temp_debug_value << std::endl;
    temp_debug_value = 0;
    std::cout << this->current_cubes->empty() << std::endl;
    if(!this->current_cubes->empty()){
        this->current_cubes->top()->temp_length=0;
        this->current_cubes->top()->print_cube();
        this->current_cubes->top()->print_solution_path();
        std::cout << this->phase4_check(this->current_cubes->top()) << std::endl;
        std::cout << "solution path length: " << this->current_cubes->top()->solution_length << " == " << this->current_cubes->top()->solution_path->size() << std::endl;
        this->solved_cube = this->current_cubes->top();
        this->current_cubes->pop();    
    }
    else{
        std::cout << "Error after phase 4" << std::endl;
        return 1;
    }

    return 0;
}

uint16_t Thistlethwaite::get_table_1_offset(BasicCube *cube){
    uint16_t index = this->edge_orientation(cube);
    return phase1_table[index];
}

uint16_t Thistlethwaite::get_table_2_offset(BasicCube *cube){
    uint16_t index_corners = corner_orientations_to_id(this->corner_orientations(cube));
    uint16_t index_edges = edge_permutation_slice_Y_to_id(this->edge_slice_permutation(cube, &this->edge_slice_Y));
    return this->phase2_table[index_edges][index_corners];
}

uint16_t Thistlethwaite::get_table_3_offset(BasicCube *cube){
    uint32_t id_c = corner_permutation_to_id(this->corner_permutation(cube));
    uint16_t id_e = edge_permutation_slice_X_to_id(this->edge_slice_permutation(cube, &this->edge_slice_X));
    return this->phase3_table[id_c][id_e];
}

uint16_t Thistlethwaite::get_table_4_offset(BasicCube *cube){
    return this->phase4_table
                    [this->corner_permutation_to_id_phase4(corner_permutation_to_id(this->corner_permutation(cube)))]
                    [this->edge_slice_permutation_to_id(cube, this->edge_slice_X)]
                    [this->edge_slice_permutation_to_id(cube, this->edge_slice_Y)]
                    [this->edge_slice_permutation_to_id(cube, this->edge_slice_Z)];
}

int Thistlethwaite::initialize_phase_3_initial_cubes(){
    std::stack<BasicCube*> cubes = std::stack<BasicCube*>();
    cubes.push(new BasicCube());
    this->phase_3_table_initial_cubes[0] = new BasicCube();
    size_t counter = 1;
    while(!cubes.empty()){
        BasicCube* cube = cubes.top();
        cubes.pop();
        for(auto move : this->moves_p3){

            if(cube->solution_length > 0 &&
                !check_move_sanity(cube->solution_path->back(), move)){
                continue;
            }

            BasicCube* child = new BasicCube(cube);
            child->rotate(move);

            if(corners_in_orbits(child, this->corner_orbit)){
                bool different = true;
                uint32_t corner_permutation = this->corner_permutation(child);
                for(size_t i = 0; i < counter; i++){
                    if(corner_permutation == this->corner_permutation(this->phase_3_table_initial_cubes[i])){
                        different = false;
                        break;
                    }
                }
                if(different){
                    BasicCube* temp = new BasicCube(child);
                    temp->clear_solution_path();
                    this->phase_3_table_initial_cubes[counter] = temp;
                    counter++;
                    if(counter>96){
                        std::cout << "ERROR: too many initial cubes for phase 3 found";
                        return 1;
                    }                    
                }
            }
            if(child->temp_length < 4){
                cubes.push(child);
            }
            else{
                delete child;
            }
        }
        delete cube;
    }
    if(counter != 96){
        std::cout << "ERROR: not enough initial cubes found for phase 3. Found: " << counter << std::endl ;
    }

    return 0;
}

uint32_t Thistlethwaite::corner_permutation(BasicCube *cube){
    uint32_t value = 0;
    for(Side left_or_right : {Side::Left, Side::Right}){
        for(Side up_or_down : {Side::Up, Side::Down}){
            for(Side front_or_back : {Side::Front, Side::Back}){
                auto destination_corner = cube->get_corner(left_or_right, up_or_down, front_or_back);
                uint16_t corner = this->get_corner_destination(destination_corner);
                uint16_t offset = this->get_corner_destination(std::make_tuple(
                    left_or_right,
                    up_or_down,
                    front_or_back
                ));
                value += corner << (offset * 3);
            }
        }
    }
    return value;
}

uint16_t Thistlethwaite::get_corner_destination(std::tuple<Side, Side, Side> corner_tuple){
    for(size_t i = 0; i < 8; i++){
        if(compare_tuple(corner_tuple, this->corner_order[i])){
            return i;
        }
    }
    return 8;
}

uint32_t Thistlethwaite::corner_permutation_to_id_phase4(uint32_t number){
    if(!permutation_to_id_lookup_table_initialized_phase_4){
        permutation_to_id_lookup_table_initialized_phase_4 = true;
        std::vector<uint16_t> initial = std::vector<uint16_t>();
        std::stack<BasicCube*> cubes = std::stack<BasicCube*>();
        cubes.push(new BasicCube());


        size_t counter = 0;
        while(!cubes.empty()){
            BasicCube* cube = cubes.top();
            cubes.pop();
            for(auto move : this->moves_p4){
                if(cube->solution_length > 0 &&
                    !check_move_sanity(cube->solution_path->back(), move)){
                    continue;
                }

                BasicCube* child = new BasicCube(cube);
                child->rotate(move);
                uint16_t corner_id = corner_permutation_to_id(this->corner_permutation(child));
                if(corners_in_orbits(child, this->corner_orbit) &&
                    std::find(initial.begin(), initial.end(), corner_id) == initial.end()){
                    initial.push_back(corner_id);
                    permutation_to_id_lookup_table_phase_4[corner_id] = counter;
                    counter++;
                }
                if(child->temp_length < 6){ // depth 6 should be enough
                    cubes.push(child);
                }
                else{
                    delete child;
                }
            }
            delete cube;
        }
        std::cout << "corner cubes ids: " << counter << std::endl;
    }
    return permutation_to_id_lookup_table_phase_4[number];
}

uint16_t Thistlethwaite::edge_slice_permutation_to_id(BasicCube* cube, std::array<std::pair<Side, Side>, 4> edge_slice){
    std::array<std::pair<Side, Side>, 4> destination_edge_slice = std::array<std::pair<Side, Side>, 4>();
    // std::cout << "Destiantion permutation (real for a time):" << std::endl;
    
    for(size_t i = 0; i < 4; i++){
        destination_edge_slice[i] = cube->get_edge(std::get<0>(edge_slice[i]), std::get<1>(edge_slice[i]));
        // std::cout << side_to_char(std::get<0>(destination_edge_slice[i])) << ", " << side_to_char(std::get<1>(destination_edge_slice[i])) << std::endl;
    }

    if(compare_pair(destination_edge_slice[0], edge_slice[0])){
        if(compare_pair(destination_edge_slice[1], edge_slice[1])){
            if(compare_pair(destination_edge_slice[2], edge_slice[2])){
                return 0;
            }
            else{
                return 1;
            }
        }
        else if(compare_pair(destination_edge_slice[1], edge_slice[2])){
            if(compare_pair(destination_edge_slice[2], edge_slice[1])){
                return 2;
            }
            else{
                return 3;
            }
        }
        else if(compare_pair(destination_edge_slice[1], edge_slice[3])){
            if(compare_pair(destination_edge_slice[2], edge_slice[1])){
                return 4;
            }
            else{
                return 5;
            }
        }
    }
    else if(compare_pair(destination_edge_slice[0], edge_slice[1])){
        if(compare_pair(destination_edge_slice[1], edge_slice[0])){
            if(compare_pair(destination_edge_slice[2], edge_slice[2])){
                return 6;
            }
            else{
                return 7;
            }
        }
        else if(compare_pair(destination_edge_slice[1], edge_slice[2])){
            if(compare_pair(destination_edge_slice[2], edge_slice[0])){
                return 8;
            }
            else{
                return 9;
            }
        }
        else if(compare_pair(destination_edge_slice[1], edge_slice[3])){
            if(compare_pair(destination_edge_slice[2], edge_slice[0])){
                return 10;
            }
            else{
                return 11;
            }
        }
    }
    else if(compare_pair(destination_edge_slice[0], edge_slice[2])){
        if(compare_pair(destination_edge_slice[1], edge_slice[0])){
            if(compare_pair(destination_edge_slice[2], edge_slice[1])){
                return 12;
            }
            else{
                return 13;
            }
        }
        else if(compare_pair(destination_edge_slice[1], edge_slice[1])){
            if(compare_pair(destination_edge_slice[2], edge_slice[0])){
                return 14;
            }
            else{
                return 15;
            }
        }
        else if(compare_pair(destination_edge_slice[1], edge_slice[3])){
            if(compare_pair(destination_edge_slice[2], edge_slice[0])){
                return 16;
            }
            else{
                return 17;
            }
        }
    }
    else if(compare_pair(destination_edge_slice[0], edge_slice[3])){
        if(compare_pair(destination_edge_slice[1], edge_slice[0])){
            if(compare_pair(destination_edge_slice[2], edge_slice[1])){
                return 18;
            }
            else{
                return 19;
            }
        }
        else if(compare_pair(destination_edge_slice[1], edge_slice[1])){
            if(compare_pair(destination_edge_slice[2], edge_slice[0])){
                return 20;
            }
            else{
                return 21;
            }
        }
        else if(compare_pair(destination_edge_slice[1], edge_slice[2])){
            if(compare_pair(destination_edge_slice[2], edge_slice[0])){
                return 22;
            }
            else{
                return 23;
            }
        }
    }
    return 24;
}

uint16_t Thistlethwaite::edge_orientation(BasicCube* cube){
    uint16_t number = this->phase1_check(cube);
    uint16_t value = 0;
    for(size_t i = 0; i < 12; i++){
        value += (number & 0b1) << (11 - i); 
        number = number >> 1;
    }
    return value;
}


static inline std::map<std::pair<Side,Side>, bool>* initialize_phase1_map(){
    std::map<std::pair<Side,Side>, bool>* new_map = new std::map<std::pair<Side,Side>, bool>();
    new_map->insert({std::make_pair(Side::Front, Side::Up), true});
    new_map->insert({std::make_pair(Side::Front, Side::Left), true});
    new_map->insert({std::make_pair(Side::Front, Side::Right), true});
    new_map->insert({std::make_pair(Side::Front, Side::Down), true});
    
    new_map->insert({std::make_pair(Side::Back, Side::Up), true});
    new_map->insert({std::make_pair(Side::Back, Side::Left), true});
    new_map->insert({std::make_pair(Side::Back, Side::Right), true});
    new_map->insert({std::make_pair(Side::Back, Side::Down), true});

    new_map->insert({std::make_pair(Side::Up, Side::Left), true});
    new_map->insert({std::make_pair(Side::Up, Side::Right), true});

    new_map->insert({std::make_pair(Side::Down, Side::Left), true});
    new_map->insert({std::make_pair(Side::Down, Side::Right), true});

    return new_map;
}


static inline bool check_move_sanity(Move* last_move, Move* new_move){
    // Let's see that there is no need to move the same side twice, as:
    //  - two same quarter moves == half move - already calculated the step before
    //  - two same half moves == empty move - no need to go back to the step before
    //  - clockwise + counterclockwise move == empty move - 
    //                  no need to go back to the step before
    //  - quarter move + half move == quarter move of opposing clockwise
    if(last_move->side == new_move->side){
        return false;
    }


    // Also when two moves don't interfere with each other 
    // then their order doesn't matter.
    // This means we can restrict moves only to come up in specific order:
    //  - only F -> B, no Front move can occure directly after Back move
    //  - only L -> R, no Left move can occure directly after Right move
    //  - only U -> D, no Up move can occure directly after Down move
    if((last_move->side == Side::Back && new_move->side == Side::Front) ||
            (last_move->side == Side::Right && new_move->side == Side::Left) ||
            (last_move->side == Side::Down && new_move->side == Side::Up)){
        return false;
    }
    return true;
}

static inline uint16_t corner_orientation_value(uint16_t number, Side clockwise, Side anticlockwise){
    number = number << 2;
    if(clockwise == Side::Left || clockwise == Side::Right){
        number += 1;
    }
    else if(anticlockwise == Side::Left || anticlockwise == Side::Right){
        number += 2;
    }
    return number;
}

static inline uint16_t corner_orientations_to_id(uint16_t number){
    uint16_t index = 0;
    // technically to count ths value for all bits this loop shoul go up to < 8,
    // however the last orientation is defined by the 7 others,
    // so there is no need to cout that
    for(uint8_t i = 0; i < 7; i++){
        index *= 3;
        index += number & 0b11;
        number = number >> 2;
    }
    return index;
}

static inline uint16_t edge_permutation_value(uint16_t number, Side side1, Side side2, std::array<std::pair<Side, Side>, 4>* edge_slice){
    number = number << 1;
    if(std::find(edge_slice->begin(), edge_slice->end(), std::make_pair(side1, side2)) == edge_slice->end() &&
            std::find(edge_slice->begin(), edge_slice->end(), std::make_pair(side2, side1)) == edge_slice->end()){
        number++;
    }
    return number;
}

static inline uint16_t edge_permutation_slice_Y_to_id(uint16_t permutation_value){
    if(!permutation_to_id_lookup_table_initialized_phase_2){
        permutation_to_id_lookup_table_initialized_phase_2 = true;
        uint16_t id = 0;
        for(uint16_t value = 0; value < 4096; value++){
            if(std::bitset<12>(value).count() == 4){
                permutation_to_id_lookup_table_phase_2[value] = id;
                id++;
            }
        }
        if(id != 495){
            std::cout << "wrong number of mapped permutations for phase 2" << std::endl;
            std::cout << id << std::endl;
        }
    }
    return permutation_to_id_lookup_table_phase_2[permutation_value];
}

static inline void print_corner_orientation(uint16_t number){
    for(uint8_t i = 0; i < 8; i++){
        std::cout << (int) (number & 0b11);
        number = number >> 2;
    } 
}

static inline bool corners_in_orbits(BasicCube* cube, std::array<std::tuple<Side, Side, Side>, 4> orbit){

    Side side1, side2, side3;

    // check left-up-back corner
    side1 = (*cube->side_left)[0][0].side_destination;
    side2 = (*cube->side_up)[0][0].side_destination;
    side3 = (*cube->side_back)[0][0].side_destination;
    if(!compare_corner_orbit_tuple(std::make_tuple(side1, side2, side3), orbit)){
        return false;
    }    

    // check left-down-front corner
    side1 = (*cube->side_left)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination;
    side2 = (*cube->side_down)[CUBE_SIZE - 1][0].side_destination;
    side3 = (*cube->side_front)[CUBE_SIZE - 1][0].side_destination;
    if(!compare_corner_orbit_tuple(std::make_tuple(side1, side2, side3), orbit)){
        return false;
    }

    // check right-down-back corner
    side1 = (*cube->side_right)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination;
    side2 = (*cube->side_down)[0][CUBE_SIZE - 1].side_destination;
    side3 = (*cube->side_back)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination;
    if(!compare_corner_orbit_tuple(std::make_tuple(side1, side2, side3), orbit)){
        return false;
    }

    // check right-up-front corner
    side1 = (*cube->side_right)[0][0].side_destination;
    side2 = (*cube->side_up)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination;
    side3 = (*cube->side_front)[0][CUBE_SIZE - 1].side_destination;
    if(!compare_corner_orbit_tuple(std::make_tuple(side1, side2, side3), orbit)){
        return false;
    }   

    return true;
}

static inline bool compare_corner_orbit_tuple(std::tuple<Side, Side, Side> tuple, std::array<std::tuple<Side, Side, Side>, 4> orbit){
    for(auto orbit_tuple : orbit){
        if(compare_tuple(tuple, orbit_tuple)){
            return true;
        }
    }
    return false;
}

static inline bool compare_tuple(std::tuple<Side, Side, Side> tuple1, std::tuple<Side, Side, Side> tuple2){
    if(!(std::get<0>(tuple1) == std::get<0>(tuple2) ||
        std::get<0>(tuple1) == std::get<1>(tuple2) ||
        std::get<0>(tuple1) == std::get<2>(tuple2))){
        return false;
    }
    if(!(std::get<1>(tuple1) == std::get<0>(tuple2) ||
        std::get<1>(tuple1) == std::get<1>(tuple2) ||
        std::get<1>(tuple1) == std::get<2>(tuple2))){
        return false;
    }
    if(!(std::get<2>(tuple1) == std::get<0>(tuple2) ||
        std::get<2>(tuple1) == std::get<1>(tuple2) ||
        std::get<2>(tuple1) == std::get<2>(tuple2))){
        return false;
    }
    return true;
}

static inline bool compare_pair(std::pair<Side, Side> pair1, std::pair<Side, Side> pair2){
    return (pair1 == pair2) || (pair1 == std::make_pair(std::get<1>(pair2), std::get<0>(pair2)));
}

static inline uint32_t corner_permutation_to_id(uint32_t number){
    uint16_t permutation[8] = {0};
    for(size_t i = 0; i < 8; i++){
        permutation[i] = number & 0b111;
        number = number >> 3;
    }

    uint32_t index = 0;
    for(size_t i = 0; i < 8; i++){
        index = index * (8 - i);
        for(size_t j = i+1; j < 8; j++){
            if(permutation[i] > permutation[j]){
                index++;
            }
        }
    }
    return index;
}

static inline uint16_t edge_permutation_slice_X_to_id(uint16_t permutation_value){
    if(!permutation_to_id_lookup_table_initialized_phase_3){
        permutation_to_id_lookup_table_initialized_phase_3 = true;
        uint16_t id = 0;
        for(uint16_t value = 0; value < 255; value++){
            if(std::bitset<12>(value).count() == 4){
                permutation_to_id_lookup_table_phase_3[value] = id;
                id++;
            }
        }
        if(id != 70){
            std::cout << "wrong number of mapped permutations for phase 3" << std::endl;
            std::cout << id << std::endl;
        }
    }
    return permutation_to_id_lookup_table_phase_3[permutation_value >> 4];
}

void debug(){
    BasicCube* cube = new BasicCube();

    // cube->rotate_sequence("0U 0R 0U 0D");
    // cube->rotate_sequence("0L 0R 0F2 0U 0L 0U 0D 0F 0R' 0D2 0F 0B2 0L' 0F 0L 0U2 0F 0L 0F2 0L 0R 0D2 0L2 0U2 0L 0U2 0B2 0L2 0U2 0L");

    Thistlethwaite* thistlethwaite = new Thistlethwaite(cube);

    std::cout << thistlethwaite->phase3_check(cube) << std::endl;

    std::cout << thistlethwaite->corner_permutation_to_id_phase4(corner_permutation_to_id(thistlethwaite->corner_permutation(cube))) << std::endl;
    std::cout << thistlethwaite->edge_slice_permutation_to_id(cube, thistlethwaite->edge_slice_X) << std::endl;
    std::cout << thistlethwaite->edge_slice_permutation_to_id(cube, thistlethwaite->edge_slice_Y) << std::endl;
    std::cout << thistlethwaite->edge_slice_permutation_to_id(cube, thistlethwaite->edge_slice_Z) << std::endl;
    // cube->print_cube();
    // cube->print_solution_path();

    delete cube;
    delete thistlethwaite;

}
