#ifndef N_CUBE_SOLVER
#define N_CUBE_SOLVER

#include <queue>
#include "../basic_cube.hpp"


class NCubeSolver {
    public:

        BasicCube* start_cube;
        BasicCube* in_between_steps_cube;
        BasicCube* reduced_cube;
        BasicCube* solved_cube;

        std::vector<Move*> all_possible_moves;

        std::priority_queue<BasicCube*, std::vector<BasicCube*>, Compare_cubes>* current_cubes;

        /**
         *  Reduces big NxNxN cube to 3x3x3 case by collecting together 
         *  all same edge and ceter pieces. Operates on start_cube 
         *  and saves the output cube into reduced_cube. 
         *
         *  1st step: complete two opposite centres
         *              Choose the two opposite sides with best percentage of solved tiles
         *  2nd step: complete two adjacent centres from the other two
         *              Choose two adjacent from remaining four with best percentage of solved tiles
         *  3rd step: complete two last centres
         *  4th step: temporarily disturb colved centres to complete 4 edges
         *              on the bottom with whatever colors are they filled the most
         *  5th step: the same as 4th with 4 edges on the top
         *  6th step: complete final 4 edges on the sides
         *
         */
        int reduce();

        /**
         *  Reducing/solving cube using commutators
         *
         *  @param[in]  option  0 for naive solving and 1 for optimized solving
         */
        int commutator_reduce(int option);


        /**
         *  Solving cube in the same way as in reducing but already putting 
         *  compeleted edges into it's final place
         */
        int solve_naive();


        int solve();

        NCubeSolver(BasicCube* cube);
        ~NCubeSolver();
    private:
        /**
         *  Performs iddfs for each step of reducing algorithm
         *
         *  @param[in]  check_step_completion   function checking if each step is completed
         *  @param[in]  step_start_cube         cube to perform iddfs on
         */
        int reduce_iddfs(size_t (NCubeSolver::*check_step_completion)(BasicCube*), BasicCube* step_start_cube);

        /**
         *  Performs single dfs - one iteration for iddfs
         *
         *  @param[in]  check_step_completion   function checking if each step is completed
         *  @param[in]  step_start_cube         cube to perform dfs on
         *  @param[in]  max_depth               max path length for this dfs iteration
         *  @param[out] solved_cube             if solution is found then solved cube is saved here
         *
         *  @return     returns 0 if solved cube is found and -1 if there is no path to solution
         */
        int reduce_single_dfs(size_t (NCubeSolver::*check_step_completion)(BasicCube*), BasicCube* step_start_cube, size_t max_depth, BasicCube** solved_cube);

        /**
         *  pair of opposite centres to be solved first
         */
        std::pair<Side, Side> choose_opposite_sides_step_1(BasicCube* cube); 
        std::pair<Side, Side> opposite_sides_step_1;
        size_t check_step_1_completion(BasicCube* cube);

        /**
         *  pair of adjacent centres to be solved next
         */
        std::pair<Side, Side> choose_adjacent_sides_step_2(BasicCube* cube);
        std::pair<Side, Side> adjacent_sides_step_2;
        size_t check_step_2_completion(BasicCube* cube);

        /**
         *  the last pair of sides
         */
        std::pair<Side, Side> choose_last_sides_step_3(BasicCube* cube);
        std::pair<Side, Side> last_sides_step_3;
        size_t check_step_3_completion(BasicCube* cube);

        /**
         *  array of four edges to be solved first in order:
         *  (Down) Front, Right, Back, Left
         */
        std::array<std::pair<Side, Side>, 4> choose_first_4_edges_step_4(BasicCube* cube);
        std::array<std::pair<Side, Side>, 4> first_4_edges_step_4;
        size_t check_step_4_completion(BasicCube* cube);

        /**
         *  array of four edges to be solved second in order:
         *  (Up) Front, Right, Back, Left
         */
        std::array<std::pair<Side, Side>, 4> choose_second_4_edges_step_5(BasicCube* cube);
        std::array<std::pair<Side, Side>, 4> second_4_edges_step_5;
        size_t check_step_5_completion(BasicCube* cube);

        /**
         *  array of last four edges to be solved last in order:
         *  Front-Left, Front-Right, Right-Back, Back-Left
         */
        std::array<std::pair<Side, Side>, 4> choose_last_4_edges_step_6(BasicCube* cube);
        std::array<std::pair<Side, Side>, 4> last_4_edges_step_6;
        size_t check_step_6_completion(BasicCube* cube);
};

#endif

/**
1 6
1 12
1 15
1 17

2 7
2 11
2 16
2 20

3 5
3 9
3 14
3 18

4 8 
4 10
4 13 
4 19

5 6 
5 8
5 22

6 7
6 21

7 8
7 23 

8 24

9 10
9 12
9 23

10 11
10 24

11 12 
11 22

12 24 

13 14 
13 16
13 21

14 15
14 23

15 16
15 24

16 22

17 18
17 20
17 21

18 22
18 19

19 20
19 24

20 23
*/