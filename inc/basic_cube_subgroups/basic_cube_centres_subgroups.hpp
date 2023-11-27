#ifndef BASIC_CUBE_CENTRES_SUBGROUPS
#define BASIC_CUBE_CENTRES_SUBGROUPS

#include "../basic_cube.hpp"
#include <map>

class BasicCubeCentresSubgroup {
    public:
        /**
         *  For each side we have 4 tiles in the same subgroup, on positions
         *  (x, y) where looking straight at side we count x from top to bottom
         *  and y from left to right:
         *  1. (x, y) - upper-left quarter
         *  2. (y, -x) - upper-right quarter
         *  3. (-x, -y) - lower-right qaurter
         *  4, (-y, x) - lower-left quarter
         */
        std::map<Side, std::array<BasicCubeTile*, 4>> subgroup;

        std::vector<std::pair<std::pair<Side, size_t>, std::pair<Side, size_t>>> swap_path;

        size_t no_of_misplaced_tiles;

        Side base_side;
        size_t base_row_index;
        size_t base_column_index;

        /**
         *  Contructs object with subgroup contatining tile of given coordinates
         *  At the moment it should be tile from upper-left quarter - it is best to take
         *  it from U, L, R, or F side
         *
         *  @param[in]  cube            cube to construct subgroup of
         *  @param[in]  side            side that includes wanted tile
         *  @param[in]  row_index       row index of wanted tile
         *  @param[in]  column_index    column index of wanted tile
         */
        BasicCubeCentresSubgroup(BasicCube* cube, Side side, size_t row_index, size_t column_index);

        /**
         *  Copying contructor
         *  
         *  @param[in]  subgroup    old object to copy
         */
        BasicCubeCentresSubgroup(BasicCubeCentresSubgroup* subgroup);

        // ~BasicCubeCentresSubgroup();

        /**
         *  Converts tile to the same-positioned tile at another side
         *  
         *  @param[in]  old_side
         *  @param[in]  old_row_index
         *  @param[in]  old_column_index
         *  @param[in]  new_side
         *  @param[out] new_row_index
         *  @param[out] new_column_index
         *
         *  @return 
         */
        int convert_subgroup_elem(Side old_side, size_t old_row_index, size_t old_column_index,
                                  Side new_side, size_t* new_row_index, size_t* new_column_index);


        /**
         *  Convert from standarized to cube index
         *
         *  @param[in]  side
         *  @param[in]  row_index
         *  @param[in]  column_index
         *  @param[out] cube_row_index
         *  @param[out] cube_column_index
         *
         *  @return 0 if succesuflly
         */
        int convert_to_cube_index(Side side, size_t row_index, size_t column_index,
                                  size_t* cube_row_index, size_t* cube_column_index);

        /**
         *  Get real indexes of element - for move generation
         */
        int get_position(Side side, size_t subgroup_index, size_t* row_index, size_t* column_index);

        /**
         *  Generates subgroup from base tile. Updates no_of_misplaced_tiles
         *
         *  @param[in]  base_cube
         *  @param[in]  base_side
         *  @param[in]  base_row_index
         *  @param[in]  base_column_index
         *
         *  @return generated subgroup
         */
        std::map<Side, std::array<BasicCubeTile*, 4>> generate_subgroup(BasicCube* base_cube, Side base_side, size_t base_row_index, size_t bse_column_index);

        /**
         *  Swaps two tiles with generating right commutator
         *  
         *  @param[in]  side1
         *  @param[in]  subgroup_index1
         *  @param[in]  side2
         *  @param[in]  subgroup_index2
         *
         *  @return     generated commutator move vector
         */
        std::vector<Move*> generate_commutator(Side side1, size_t subgroup_index1, Side side2, size_t subgroup_index2);

        /**
         *  Solves this permutation by just swapping two tiles 
         *  greedily - it iterates over each tile and sends it to the 'free'
         *  (not containing solved tile) place at its destination side
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

};

#endif