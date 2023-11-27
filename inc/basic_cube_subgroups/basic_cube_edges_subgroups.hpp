#ifndef BASIC_CUBE_EDGES_SUBGROUPS
#define BASIC_CUBE_EDGES_SUBGROUPS


#include "../basic_cube.hpp"
#include <map>

class BasicCubeEdgesSubgroup {
    public:
        /**
         *  For each edge we have 2 edge parts (tile pairs) in the same subgroup, on position
         *  x where we count tile from left to right, from up to down and from ront to back
         *  
         *  1. (x) - 'left' side, x < n/2
         *  2. (-x) - 'rught' side, -x > n/2
         */
        std::map<std::pair<Side, Side>, std::array<std::pair<Side, Side>, 2>> subgroup;

        std::vector<std::pair<std::pair<std::pair<Side, Side>, size_t>, std::pair<std::pair<Side, Side>, size_t>>> swap_path;

        size_t no_of_misplaced_edge_parts;

        std::pair<Side, Side> base_edge;
        size_t base_index;

        bool is_singular;

        /**
         *  Contructs object with subgroup contatining edge part of given coordinates
         *  At the moment it should be edge part from front-up edge
         *
         *  @param[in]  cube            cube to construct subgroup of
         *  @param[in]  edge            edge that includes wanted tile
         *  @param[in]  rindex          index of wanted edge part
         */
        BasicCubeEdgesSubgroup(BasicCube* cube, std::pair<Side, Side> edge, size_t index, bool is_singular);

        /**
         *  Copying contructor
         *  
         *  @param[in]  subgroup    old object to copy
         */
        BasicCubeEdgesSubgroup(BasicCubeEdgesSubgroup* subgroup);

        // ~BasicCubeEdgesSubgroup();

        /**
         *  Converts tile to the same-positioned tile at another side
         *  
         *  @param[in]  old_edge
         *  @param[in]  old_index
         *  @param[in]  new_edge
         *  @param[out] new_index
         *
         *  @return 
         */
        int convert_subgroup_elem(std::pair<Side, Side> old_edge, size_t old_index,
                                  std::pair<Side, Side> new_edge, size_t* new_index);


        // /**
        //  *  Convert from standarized to cube index
        //  *
        //  *  @param[in]  side
        //  *  @param[in]  row_index
        //  *  @param[in]  column_index
        //  *  @param[out] cube_row_index
        //  *  @param[out] cube_column_index
        //  *
        //  *  @return 0 if succesuflly
        //  */
        // int convert_to_cube_index(Side side, size_t row_index, size_t column_index,
        //                           size_t* cube_row_index, size_t* cube_column_index);

        /**
         *  Get real indexes of element - for move generation
         */
        int get_position(std::pair<Side, Side> edge, size_t subgroup_index, size_t* row_index_side1, size_t* column_index_side1, size_t* row_index_side2, size_t* column_index_side2);

        /**
         *  Generates subgroup from base tile. Updates no_of_misplaced_tiles
         *
         *  @param[in]  base_cube
         *
         *  @return generated subgroup
         */
        std::map<std::pair<Side, Side>, std::array<std::pair<Side, Side>, 2>> generate_subgroup(BasicCube* base_cube);

        /**
         *  Swaps two tiles with generating right commutor
         *  
         *  @param[in]  edge1
         *  @param[in]  subgroup_index1
         *  @param[in]  edge2
         *  @param[in]  subgroup_index2
         *
         *  @return     generated commutator move vector
         */
        std::vector<Move*> generate_commutator(std::pair<Side, Side> edge1, size_t subgroup_index1, std::pair<Side, Side> edge2, size_t subgroup_index2);

        /**
         *  Solves this permutation by just swapping two edge_parts 
         *  greedily - it iterates over each edge part and sends it to the 'free'
         *  (not containing solved edge part) place at its destination side
         */
        int naive_solve();

        /**
         *  Solves this permutation in (hopefully) the least number of swaps
         *  and saves those swaps into swap_path vector
         *
         *  @return 0 if solved succefully 1 otherwise
         */
        int optimal_solve();

        int apply_to_cube(BasicCube* cube);

        int print_subgroup();

};

#endif 