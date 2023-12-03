#ifndef THISTLETHWAITE_HPP
#define THISTLETHWAITE_HPP

#include "basic_cube.hpp"
#include "basic_cube_3.hpp"
#include "move.hpp"
#include <queue>
#include <map>
#include <array>
#include <utility>
#include <stack>

#define PHASE_1_MAX_NO_OF_MOVES 7
#define PHASE_2_MAX_NO_OF_MOVES 10
#define PHASE_3_MAX_NO_OF_MOVES 13
#define PHASE_4_MAX_NO_OF_MOVES 15

#define PHASE_1_TABLE_DIR_NAME "tables/phase_1.txt"
#define PHASE_2_TABLE_DIR_NAME "tables/phase_2.txt"
#define PHASE_3_TABLE_DIR_NAME "tables/phase_3.txt"
#define PHASE_4_TABLE_DIR_NAME "tables/phase_4.txt"

void debug();

class Thistlethwaite {
    public:
        uint16_t compare_cubes_heuristics(BasicCube3* cube1, BasicCube3* cube2);

        BasicCube3* start_cube;
        BasicCube3* solved_cube;
        size_t phase1_move_count;
        // auto phase1_time;
        size_t phase2_move_count;
        // auto phase2_time
        size_t phase3_move_count;
        // auto phase3_time
        size_t phase4_move_count;
        // auto phase3_time

        std::vector<Move*>* previous_moves;
        size_t previous_moves_count;

        std::priority_queue<BasicCube3*, std::vector<BasicCube3*>, Compare_cubes_3>* current_cubes;
        std::vector<Move*> moves_p1;
        std::vector<Move*> moves_p2;
        std::vector<Move*> moves_p3;
        std::vector<Move*> moves_p4;

        int solve();

        Thistlethwaite(BasicCube* cube);
        ~Thistlethwaite();

    // private:
        std::map<std::pair<Side,Side>, bool>* edge_orientation_map;

        std::array<std::pair<Side, Side>, 4> edge_slice_X = {
            std::make_pair(Side::Left, Side::Front),
            std::make_pair(Side::Front, Side::Right),
            std::make_pair(Side::Right, Side::Back),
            std::make_pair(Side::Back, Side::Left)
        };
        
        std::array<std::pair<Side, Side>, 4> edge_slice_Y = {
            std::make_pair(Side::Up, Side::Front),
            std::make_pair(Side::Front, Side::Down),
            std::make_pair(Side::Down, Side::Back),
            std::make_pair(Side::Back, Side::Up)
        };

        std::array<std::pair<Side, Side>, 4> edge_slice_Z = {
            std::make_pair(Side::Up, Side::Right),
            std::make_pair(Side::Right, Side::Down),
            std::make_pair(Side::Down, Side::Left),
            std::make_pair(Side::Left, Side::Up)
        };

        std::array<std::tuple<Side, Side, Side>, 4> corner_orbit = {
            std::make_tuple(Side::Left, Side::Up, Side::Back),
            std::make_tuple(Side::Left, Side::Down, Side::Front),
            std::make_tuple(Side::Right, Side::Down, Side::Back),
            std::make_tuple(Side::Right, Side::Up, Side::Front)
        };

        std::array<std::tuple<Side, Side, Side>, 8> corner_order = {
            std::make_tuple(Side::Left, Side::Up, Side::Back),
            std::make_tuple(Side::Left, Side::Down, Side::Front),
            std::make_tuple(Side::Right, Side::Down, Side::Back),
            std::make_tuple(Side::Right, Side::Up, Side::Front),
            std::make_tuple(Side::Left, Side::Up, Side::Front),
            std::make_tuple(Side::Left, Side::Down, Side::Back),
            std::make_tuple(Side::Right, Side::Down, Side::Front),
            std::make_tuple(Side::Right, Side::Up, Side::Back)
        };

        
        int bfs(std::vector<Move*>* allowed_moves,
                uint16_t (Thistlethwaite::*check_function)(BasicCube3*),
                uint16_t (Thistlethwaite::*get_table_offset)(BasicCube3*),
                uint16_t max_depth);
        
        int clear_current_cubes();

        uint16_t edge_orientation(BasicCube3* cube);

        uint16_t corner_orientations(BasicCube3* cube);
        uint16_t edge_slice_permutation(BasicCube3* cube, std::array<std::pair<Side, Side>, 4>* edge_slice);

        uint32_t corner_permutation(BasicCube3* cube);
        uint16_t get_corner_destination(std::tuple<Side, Side, Side> corner_tuple);

        uint16_t edge_slice_permutation_to_id(BasicCube3* cube, std::array<std::pair<Side, Side>, 4> edge_slice);

        int initialize_phase_3_initial_cubes();

        int generate_table_phase_1();
        int generate_table_phase_2();
        int generate_table_phase_3();
        int generate_table_phase_4();

        int print_table_phase_1();
        int print_table_phase_2();
        int print_table_phase_3();
        int print_table_phase_4();
        
        int read_table_phase_1();
        int read_table_phase_2();
        int read_table_phase_3();
        int read_table_phase_4();

        uint16_t get_table_1_offset(BasicCube3* cube);
        uint16_t get_table_2_offset(BasicCube3* cube);
        uint16_t get_table_3_offset(BasicCube3* cube);
        uint16_t get_table_4_offset(BasicCube3* cube);

        size_t phase1_table[2048] = {0};

        size_t phase2_table[495][2187] = {0};

        BasicCube3* phase_3_table_initial_cubes[96] = {0};
        size_t phase3_table[40320][70] = {0};

        size_t phase4_table[96][24][24][24] = {0};

        uint32_t corner_permutation_to_id_phase4(uint32_t number);

        uint16_t phase1_check(BasicCube3* cube);
        uint16_t phase2_check(BasicCube3* cube);
        uint16_t phase3_check(BasicCube3* cube);
        uint16_t phase4_check(BasicCube3* cube);
};

#endif