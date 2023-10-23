#ifndef THISTLETHWAITE_HPP
#define THISTLETHWAITE_HPP

#include "basic_cube.hpp"
#include "move.hpp"
#include <queue>
#include <map>
#include <array>
#include <utility>


class Thistlethwaite {
    public:
        BasicCube* start_cube;
        BasicCube* solved_cube;
        uint8_t phase1_move_count;
        // auto phase1_time;
        uint8_t phase2_move_count;
        // auto phase2_time
        uint8_t phase3_move_count;
        // auto phase3_time
        uint8_t phase4_move_count;
        // auto phase3_time

        std::queue<BasicCube*>* current_cubes;
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

        
        int bfs(std::vector<Move*>* allowed_moves, int (Thistlethwaite::*check_function)(BasicCube*));
        
        int clear_current_cubes();

        int phase1_check(BasicCube* cube);
        int phase2_check(BasicCube* cube);
        int phase3_check(BasicCube* cube);
        int phase4_check(BasicCube* cube);
};

#endif