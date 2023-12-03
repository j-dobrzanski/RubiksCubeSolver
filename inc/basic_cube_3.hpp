#ifndef BASIC_CUBE_3_HPP
#define BASIC_CUBE_3_HPP

#include "basic_cube.hpp"

#define CUBE_SIZE_3 3
#define CUBE_SIZE_LAST_INDEX_3 2
#define EDGE_LENGTH_3 1


class BasicCube3{
    public:
        std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side_front;
        std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side_back;
        std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side_left;
        std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side_right;
        std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side_up;
        std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side_down;

        std::map<Side, std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>*> sides;

        std::vector<Move*>* solution_path;
        uint64_t solution_length;
        uint16_t temp_length;
        double temp_length_plus_heuristic;

        BasicCube3();
        BasicCube3(BasicCube3* cube3);
        ~BasicCube3();
        BasicCube3(BasicCube* cube);

        int clear_solution_path();
        int print_solution_path();

        int check_solution();
        void print_cube();
        int compare_cube(BasicCube3* cube);
        int rotate(Move* move);

        int rotate_left(size_t tile_index, bool clockwise);
        int rotate_right(size_t tile_index, bool clockwise);
        int rotate_front(size_t tile_index, bool clockwise);
        int rotate_back(size_t tile_index, bool clockwise);
        int rotate_up(size_t tile_index, bool clockwise);
        int rotate_down(size_t tile_index, bool clockwise);

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
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_single_edge(Side side1, Side side2);


        /**
         *  Gives current state of Left-Up edge
         *  Edge is ordered naturally - from Front to Back
         *  First element from each pair is destination on Left side,
         *  second element is destiantion from Up side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_LU();

        /**
         *  Gives current state of Left-Down edge
         *  Edge is ordered naturally - from Front to Back
         *  First element from each pair is destination on Left side,
         *  second element is destiantion from Down side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_LD();

        /**
         *  Gives current state of Right-Down edge
         *  Edge is ordered naturally - from Front to Back
         *  First element from each pair is destination on Right side,
         *  second element is destiantion from Down side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_RD();

        /**
         *  Gives current state of Right-Up edge
         *  Edge is ordered naturally - from Front to Back
         *  First element from each pair is destination on Right side,
         *  second element is destiantion from Up side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_RU();


        /**
         *  Gives current state of Left-Back edge
         *  Edge is ordered naturally - from Up to Down
         *  First element from each pair is destination on Left side,
         *  second element is destiantion from Back side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_LB();

        /**
         *  Gives current state of Left-Front edge
         *  Edge is ordered naturally - from Up to Down
         *  First element from each pair is destination on Left side,
         *  second element is destiantion from Front side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_LF();
        
        /**
         *  Gives current state of Right-Front edge
         *  Edge is ordered naturally - from Up to Down
         *  First element from each pair is destination on Right side,
         *  second element is destiantion from Front side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_RF();
        
        /**
         *  Gives current state of Right-Back edge
         *  Edge is ordered naturally - from Up to Down
         *  First element from each pair is destination on Right side,
         *  second element is destiantion from Back side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_RB();

        
        /**
         *  Gives current state of Front-Up edge
         *  Edge is ordered naturally - from Left to Right
         *  First element from each pair is destination on Front side,
         *  second element is destiantion from Up side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_FU();
        
        /**
         *  Gives current state of Front-Down edge
         *  Edge is ordered naturally - from Left to Right
         *  First element from each pair is destination on Front side,
         *  second element is destiantion from Down side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_FD();
        
        /**
         *  Gives current state of Back-Down edge
         *  Edge is ordered naturally - from Left to Right
         *  First element from each pair is destination on Back side,
         *  second element is destiantion from Down side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_BD();
        
        /**
         *  Gives current state of Back-Up edge
         *  Edge is ordered naturally - from Left to Right
         *  First element from each pair is destination on Back side,
         *  second element is destiantion from Up side.
         *
         *  @return     Array of pairs of side destinations in given edge
         */
        std::array<std::pair<Side, Side>, EDGE_LENGTH_3> get_edge_BU();
    private:
        std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* initialize_side_from_cube(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side);
        std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* initialize_side(Side side);
};

class Compare_cubes_3 {
    public:
    bool operator()(BasicCube3* cube1, BasicCube3* cube2) {
        return cube1->temp_length_plus_heuristic < cube2->temp_length_plus_heuristic;
    }
};

#endif