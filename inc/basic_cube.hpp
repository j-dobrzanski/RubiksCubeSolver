#ifndef BASIC_CUBE_HPP
#define BASIC_CUBE_HPP

#include "cube.hpp"
#include "basic_cube_tile.hpp"
#include <cstdint>
#include <map>
#include <utility>
#include <vector>
#include <tuple>

#include <array>

#include "../config.hpp"

// #define CUBE_SIZE 13
#define EDGE_LENGTH (CUBE_SIZE - 2)
#define CUBE_SIZE_LAST_INDEX (CUBE_SIZE - 1)

/**
 *  All edges of cube in specific order
 */
extern std::array<std::pair<Side, Side>, 12> ordered_edge_set;

/**
 *  All edges of cube in no specific order
 */
extern std::array<std::pair<Side, Side>, 24> unordered_edge_set;

/**
 *  Helper function to compare unordered edges
 *
 *  @param[in]  pair1   first pair of sides in no order
 *  @param[in]  pair2   second pair of sides in no order
 *
 *  @return     true if pairs are equal (even if in dofferent order)
 */
bool compare_edge(std::pair<Side, Side> pair1, std::pair<Side, Side> pair2);

/**
 *  Helper function to compare unordered corners
 *
 *  @param[in]  tuple1  first tuple of 3 sides in no order
 *  @param[in]  tuple2  second tuple of 3 sides in no order
 *
 *  @return     true if tuples are equal (even if in dofferent order)
 */
bool compare_corner(std::tuple<Side, Side, Side> tuple1, std::tuple<Side, Side, Side> tuple2);

/**
 *  Helper function to normalize order of sides when describing an edge
 *  (as in ordered_edge_set)
 *
 *  @param[in]  edge    edge to be normalized (ordered)
 *
 *  @return     normalized edge
 */
std::pair<Side, Side> normalize_edge(std::pair<Side, Side> edge);

/**
 *  Helper function to normalize order of sides when describing a corner
 *  (in order Left/Right -> Front/Back -> Up/Down).
 *  It expects a valid corner - one side from each pair.
 *
 *  @param[in]  corner    corner to be normalized (ordered)
 *
 *  @return     normalized corner
 */
std::tuple<Side, Side, Side> normalize_corner(std::tuple<Side, Side, Side> corner);

/**
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
        std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side_front;
        std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side_back;
        std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side_left;
        std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side_right;
        std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side_up;
        std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side_down;

        std::map<Side, std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>*> sides;

        std::vector<Move*>* solution_path;
        uint64_t solution_length;
        uint16_t temp_length;
        double temp_length_plus_heuristic;

        BasicCube();
        BasicCube(BasicCube* cube);
        int clear_solution_path();
        int print_solution_path();

        int check_solution();
        void print_cube();
        int rotate(Move* move);
        int rotate_left(size_t tile_index, bool clockwise);
        int rotate_right(size_t tile_index, bool clockwise);
        int rotate_front(size_t tile_index, bool clockwise);
        int rotate_back(size_t tile_index, bool clockwise);
        int rotate_up(size_t tile_index, bool clockwise);
        int rotate_down(size_t tile_index, bool clockwise);
        int compare_cube(BasicCube* cube);

        int rotate_sequence(std::string move_sequence);

        std::tuple<Side, Side, Side> get_corner(Side left_or_right, Side up_or_down, Side front_or_back);
        
        /**
         *  Always assumes that whole edge is solved,
         *  e.g. it consists of edge pieces of the same colors in the same orientations.
         *  Returns just middle piece in case of CUBE_SIZE 3
         *
         *  @param[in]      side1   one side of wanted edge in no particular order
         *  @param[in]      side2   another side of wanted edge in no particular order
         *
         *  @return     Destination sides of wanted solved edge
         */
        std::pair<Side, Side> get_solved_edge(Side side1, Side side2);


        /**
         *  Gives current state of whole given edge 
         *  Edge is ordered naturally - from up to down, from left to right or from front to back
         *  looking from front side perspective
         *
         *  @param[in]      side1   one side of wanted edge - goes into 1st position in each pair 
         *  @param[in]      side2   another side of wanted edge - goes into 2nd position in each pair
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_single_edge(Side side1, Side side2);


        /**
         *  Gives current state of Left-Up edge
         *  Edge is ordered naturally - from Front to Back
         *  First element from each pair is destination on Left side,
         *  second element is destiantion from Up side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_LU();

        /**
         *  Gives current state of Left-Down edge
         *  Edge is ordered naturally - from Front to Back
         *  First element from each pair is destination on Left side,
         *  second element is destiantion from Down side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_LD();

        /**
         *  Gives current state of Right-Down edge
         *  Edge is ordered naturally - from Front to Back
         *  First element from each pair is destination on Right side,
         *  second element is destiantion from Down side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_RD();

        /**
         *  Gives current state of Right-Up edge
         *  Edge is ordered naturally - from Front to Back
         *  First element from each pair is destination on Right side,
         *  second element is destiantion from Up side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_RU();


        /**
         *  Gives current state of Left-Back edge
         *  Edge is ordered naturally - from Up to Down
         *  First element from each pair is destination on Left side,
         *  second element is destiantion from Back side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_LB();

        /**
         *  Gives current state of Left-Front edge
         *  Edge is ordered naturally - from Up to Down
         *  First element from each pair is destination on Left side,
         *  second element is destiantion from Front side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_LF();
        
        /**
         *  Gives current state of Right-Front edge
         *  Edge is ordered naturally - from Up to Down
         *  First element from each pair is destination on Right side,
         *  second element is destiantion from Front side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_RF();
        
        /**
         *  Gives current state of Right-Back edge
         *  Edge is ordered naturally - from Up to Down
         *  First element from each pair is destination on Right side,
         *  second element is destiantion from Back side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_RB();

        
        /**
         *  Gives current state of Front-Up edge
         *  Edge is ordered naturally - from Left to Right
         *  First element from each pair is destination on Front side,
         *  second element is destiantion from Up side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_FU();
        
        /**
         *  Gives current state of Front-Down edge
         *  Edge is ordered naturally - from Left to Right
         *  First element from each pair is destination on Front side,
         *  second element is destiantion from Down side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_FD();
        
        /**
         *  Gives current state of Back-Down edge
         *  Edge is ordered naturally - from Left to Right
         *  First element from each pair is destination on Back side,
         *  second element is destiantion from Down side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_BD();
        
        /**
         *  Gives current state of Back-Up edge
         *  Edge is ordered naturally - from Left to Right
         *  First element from each pair is destination on Back side,
         *  second element is destiantion from Up side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH> get_edge_BU();

        ~BasicCube();

    private:
        std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* initialize_side(Side side);
        int check_solution_basic();
        int check_solution_picture();
        
};

class Compare_cubes {
    public:
    bool operator()(BasicCube* cube1, BasicCube* cube2) {
        return cube1->temp_length_plus_heuristic < cube2->temp_length_plus_heuristic;
    }
};


#endif