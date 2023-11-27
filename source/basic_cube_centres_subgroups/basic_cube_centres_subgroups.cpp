#include "../../inc/basic_cube_subgroups/basic_cube_centres_subgroups.hpp"
#include <iostream>
#include <utility>
#include <vector>

static std::array<std::pair<Side, Side>, 3> opposite_sides = {
    std::make_pair(Side::Left, Side::Right),
    std::make_pair(Side::Front, Side::Back),
    std::make_pair(Side::Up, Side::Down)
};

static std::map<std::pair<Side, Side>, Side> opposite_sides_map = {
    {std::make_pair(Side::Left, Side::Right), Side::Front},
    {std::make_pair(Side::Front, Side::Back), Side::Right},
    {std::make_pair(Side::Up, Side::Down), Side::Right},
    {std::make_pair(Side::Right, Side::Left), Side::Back},
    {std::make_pair(Side::Back, Side::Front), Side::Left},
    {std::make_pair(Side::Down, Side::Up), Side::Left}
};

static std::map<std::pair<Side, Side>, Side> adjacent_sides_map = {
    {std::make_pair(Side::Left, Side::Up), Side::Front},
    {std::make_pair(Side::Left, Side::Down), Side::Back},
    {std::make_pair(Side::Right, Side::Down), Side::Front},
    {std::make_pair(Side::Right, Side::Up), Side::Back},
    {std::make_pair(Side::Left, Side::Back), Side::Up},
    {std::make_pair(Side::Left, Side::Front), Side::Down},
    {std::make_pair(Side::Right, Side::Front), Side::Up},
    {std::make_pair(Side::Right, Side::Back), Side::Down},
    {std::make_pair(Side::Front, Side::Up), Side::Right},
    {std::make_pair(Side::Front, Side::Down), Side::Left},
    {std::make_pair(Side::Back, Side::Down), Side::Right},
    {std::make_pair(Side::Back, Side::Up), Side::Left}
};

BasicCubeCentresSubgroup::BasicCubeCentresSubgroup(BasicCube* cube, Side side, size_t row_index, size_t column_index){
    this->base_side = side;
    this->base_row_index = row_index;
    this->base_column_index = column_index;
    this->no_of_misplaced_tiles = 0;
    this->subgroup = this->generate_subgroup(cube, side, row_index, column_index);
}

BasicCubeCentresSubgroup::BasicCubeCentresSubgroup(BasicCubeCentresSubgroup* subgroup){
    this->base_side = subgroup->base_side;
    this->base_row_index = subgroup->base_row_index;
    this->base_column_index = subgroup->base_column_index;

    for(auto side : subgroup->subgroup){
        for(size_t i= 0; i < 4; i++){
            this->subgroup[side.first][i] = side.second[i];
        }
    }
}

int BasicCubeCentresSubgroup::convert_subgroup_elem(Side old_side, size_t old_row_index, size_t old_column_index, Side new_side, size_t *new_row_index, size_t *new_column_index){
    switch(old_side){
        case Side::Left :
            switch(new_side){
                case Side::Left :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Right :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Front :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Back :
                    *new_row_index = old_row_index;
                    *new_column_index = CUBE_SIZE_LAST_INDEX - old_column_index;
                    return 0;
                case Side::Up :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Down :
                    *new_row_index = CUBE_SIZE_LAST_INDEX - old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Error :
                    *new_row_index = -1;
                    *new_column_index = 1;
                    return 1;
            }
            break;
        case Side::Right :
            switch(new_side){
                case Side::Left :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Right :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Front :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Back :
                    *new_row_index = old_row_index;
                    *new_column_index = CUBE_SIZE_LAST_INDEX - old_column_index;
                    return 0;
                case Side::Up :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Down :
                    *new_row_index = CUBE_SIZE_LAST_INDEX - old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Error :
                    *new_row_index = -1;
                    *new_column_index = 1;
                    return 1;
            }
            break;
        case Side::Front :
            switch(new_side){
                case Side::Left :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Right :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Front :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Back :
                    *new_row_index = old_row_index;
                    *new_column_index = CUBE_SIZE_LAST_INDEX - old_column_index;
                    return 0;
                case Side::Up :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Down :
                    *new_row_index = CUBE_SIZE_LAST_INDEX - old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Error :
                    *new_row_index = -1;
                    *new_column_index = 1;
                    return 1;
            }
            break;
        case Side::Back :
            switch(new_side){
                case Side::Left :
                    *new_row_index = old_row_index;
                    *new_column_index = CUBE_SIZE_LAST_INDEX - old_column_index;
                    return 0;
                case Side::Right :
                    *new_row_index = old_row_index;
                    *new_column_index = CUBE_SIZE_LAST_INDEX - old_column_index;
                    return 0;
                case Side::Front :
                    *new_row_index = old_row_index;
                    *new_column_index = CUBE_SIZE_LAST_INDEX - old_column_index;
                    return 0;
                case Side::Back :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Up :
                    *new_row_index = old_row_index;
                    *new_column_index = CUBE_SIZE_LAST_INDEX - old_column_index;
                    return 0;
                case Side::Down :
                    *new_row_index = CUBE_SIZE_LAST_INDEX - old_row_index;
                    *new_column_index = CUBE_SIZE_LAST_INDEX - old_column_index;
                    return 0;
                case Side::Error :
                    *new_row_index = -1;
                    *new_column_index = 1;
                    return 1;
            }
            break;
        case Side::Up :
            switch(new_side){
                case Side::Left :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Right :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Front :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Back :
                    *new_row_index = old_row_index;
                    *new_column_index = CUBE_SIZE_LAST_INDEX - old_column_index;
                    return 0;
                case Side::Up :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Down :
                    *new_row_index = CUBE_SIZE_LAST_INDEX - old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Error :
                    *new_row_index = -1;
                    *new_column_index = 1;
                    return 1;
            }
            break;
        case Side::Down :
            switch(new_side){
                case Side::Left :
                    *new_row_index = CUBE_SIZE_LAST_INDEX - old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Right :
                    *new_row_index = CUBE_SIZE_LAST_INDEX - old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Front :
                    *new_row_index = CUBE_SIZE_LAST_INDEX - old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Back :
                    *new_row_index = CUBE_SIZE_LAST_INDEX - old_row_index;
                    *new_column_index = CUBE_SIZE_LAST_INDEX - old_column_index;
                    return 0;
                case Side::Up :
                    *new_row_index = CUBE_SIZE_LAST_INDEX - old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Down :
                    *new_row_index = old_row_index;
                    *new_column_index = old_column_index;
                    return 0;
                case Side::Error :
                    *new_row_index = -1;
                    *new_column_index = 1;
                    return 1;
            }
            break;
        case Side::Error :
            *new_row_index = -1;
            *new_column_index = 1;
            return 1;
    }
    return 1;
}

int BasicCubeCentresSubgroup::convert_to_cube_index(Side side, size_t row_index, size_t column_index,
                                                    size_t* cube_row_index, size_t* cube_column_index){
    switch(side){
        case Side::Left :
        case Side::Right : 
        case Side::Front :
        case Side::Up :
            *cube_row_index = row_index;
            *cube_column_index = column_index;
            return 0;
        case Side::Back :
            *cube_row_index = row_index;
            *cube_column_index = CUBE_SIZE_LAST_INDEX - column_index;
            return 0;
        case Side::Down :
            *cube_row_index = CUBE_SIZE_LAST_INDEX - row_index;
            *cube_column_index = column_index;
            return 0;
        default:
            return 1;
    }
    
}

int BasicCubeCentresSubgroup::get_position(Side side, size_t subgroup_index, size_t* row_index, size_t* column_index){
    (void) side;
    switch(subgroup_index){
        case 0:
            *row_index = this->base_row_index;
            *column_index = this->base_column_index;
            return 0;
        case 1:
            *row_index = this->base_column_index;
            *column_index = CUBE_SIZE_LAST_INDEX - this->base_row_index;
            return 0;
        case 2:
            *row_index = CUBE_SIZE_LAST_INDEX - this->base_row_index;
            *column_index = CUBE_SIZE_LAST_INDEX - this->base_column_index;
            return 0;
        case 3:
            *row_index = CUBE_SIZE_LAST_INDEX - this->base_column_index;
            *column_index = this->base_row_index;
            return 0;
        default:
            return 1;  
    }


}

std::map<Side, std::array<BasicCubeTile*, 4>> BasicCubeCentresSubgroup::generate_subgroup(BasicCube* base_cube, Side base_side, size_t base_row_index, size_t base_column_index){
    std::map<Side, std::array<BasicCubeTile*, 4>> generated_subgroup = std::map<Side, std::array<BasicCubeTile*, 4>>();

    (void) base_side;
    (void) base_row_index;
    (void) base_column_index;
    Side sides[6] = {
        Side::Left,
        Side::Right,
        Side::Front,
        Side::Back,
        Side::Up,
        Side::Down
    };
    for(Side side : sides){
        for(size_t i = 0; i < 4; i++){
            size_t row_index = 0;
            size_t column_index = 0;
            this->get_position(side, i, &row_index, &column_index);
            this->convert_to_cube_index(side, row_index, column_index, &row_index, &column_index);
            generated_subgroup[side][i] = &(*base_cube->sides[side])[row_index][column_index];
            if(side != generated_subgroup[side][i]->side_destination){
                this->no_of_misplaced_tiles++;
            }
        }
    }
    return generated_subgroup;    
}

std::vector<Move*> BasicCubeCentresSubgroup::generate_commutator(Side side1, size_t subgroup_index1, Side side2, size_t subgroup_index2){
    std::vector<Move*> moves = std::vector<Move*>();
    std::vector<Move*> append_moves = std::vector<Move*>();
    bool use_double_moves = false;
    Side pseudo_up_back = side2;
    Side pseudo_right;
    size_t column = this->base_column_index;
    size_t row = this->base_row_index;
    // (void) side1;
    // (void) subgroup_index1;
    // (void) side2;
    // (void) subgroup_index2;
    // /**
    //  *  TODO: generate commutator for given swap. Some kind of look up with correct indexes
    //  */

    if(compare_edge(std::make_pair(side1, side2), opposite_sides[0]) ||
        compare_edge(std::make_pair(side1, side2), opposite_sides[1]) ||
        compare_edge(std::make_pair(side1, side2), opposite_sides[2])){
        pseudo_right = opposite_sides_map[std::make_pair(side1, side2)];

        if(subgroup_index2 == 0){
            moves.push_back(new Move(side2, 0, true, false));
            append_moves.push_back(new Move(side2, 0, false, false));
        }
        else if(subgroup_index2 == 2){
            moves.push_back(new Move(side2, 0, false, false));
            append_moves.push_back(new Move(side2, 0, true, false));
        }
        else if(subgroup_index2 == 3){
            moves.push_back(new Move(side2, 0, true, true));
            append_moves.push_back(new Move(side2, 0, true, true));
        }
    }
    else{
        pseudo_right = adjacent_sides_map[std::make_pair(side1, side2)];
        if(subgroup_index2 == 0){
            moves.push_back(new Move(side2, 0, false, false));
            append_moves.push_back(new Move(side2, 0, true, false));
        }
        else if(subgroup_index2 == 1){
            moves.push_back(new Move(side2, 0, true, true));
            append_moves.push_back(new Move(side2, 0, true, true));
        }
        else if(subgroup_index2 == 2){
            moves.push_back(new Move(side2, 0, true, false));
            append_moves.push_back(new Move(side2, 0, false, false));
        }
    }

    if(subgroup_index1 == 1){
        moves.push_back(new Move(side1, 0, false, false));
        append_moves.push_back(new Move(side1, 0, true, false));
    }
    else if(subgroup_index1 == 2){
        moves.push_back(new Move(side1, 0, true, true));
        append_moves.push_back(new Move(side1, 0, true, true));
    }
    else if(subgroup_index1 == 3){
        moves.push_back(new Move(side1, 0, true, false));
        append_moves.push_back(new Move(side1, 0, false, false));
    }

    moves.push_back(new Move(pseudo_right, row, true, use_double_moves));
    moves.push_back(new Move(pseudo_up_back, 0, true, false));
    moves.push_back(new Move(pseudo_right, column, true, use_double_moves));
    moves.push_back(new Move(pseudo_up_back, 0, false, false));
    moves.push_back(new Move(pseudo_right, row, false, use_double_moves));
    moves.push_back(new Move(pseudo_up_back, 0, true, false));
    moves.push_back(new Move(pseudo_right, column, false, use_double_moves));
    moves.push_back(new Move(pseudo_up_back, 0, false, false));


    
    for(auto move = append_moves.rbegin(); move != append_moves.rend(); move++){
        moves.push_back(*move);
    }

    return moves;
}

int BasicCubeCentresSubgroup::naive_solve(){
    while(this->no_of_misplaced_tiles > 0){
        auto old = this->no_of_misplaced_tiles;
        // std::cout << "Naive_solve: Start of while loop with no_of_misplaced_tiles: " << this->no_of_misplaced_tiles << std::endl;
        for(auto side : this->subgroup){
            for(size_t i = 0; i < 4; i++){
                if(side.first != side.second[i]->side_destination){ // found misplaced tile
                    // std::cout << "###############################" << std::endl;
                    // std::cout << "Found swap: " << side_to_char(side.first) << ", " << i << std::endl;
                    Side destination = side.second[i]->side_destination;
                    for(size_t j = 0; j < 4; j++){
                        if(destination != this->subgroup[destination][j]->side_destination){ // found place for misplaced tile
                            // std::cout << side_to_char(destination) << ", " << j << std::endl;
                            swap_path.push_back(std::make_pair(std::make_pair(side.first, i), std::make_pair(destination, j)));
                            // for(auto side : this->subgroup){
                            //     std::cout << side_to_char(side.first) << std::endl;
                            //     for(auto val : side.second){
                            //         std::cout << "[" << side_to_char(val->side_destination) << "]"; 
                            //     }
                            //     std::cout << std::endl;
                            // }
                            // std::cout << side.second[i] << " <-> " << this->subgroup[destination][j] << std::endl; 
                            auto temp = side.second[i];
                            // std::cout << "1 step: " << side_to_char(temp->side_destination) << ", " << side_to_char(side.second[i]->side_destination) << ", " << side_to_char(this->subgroup[destination][j]->side_destination) << std::endl;
                            this->subgroup[side.first][i] = this->subgroup[destination][j];
                            // std::cout << "2 step: " << side_to_char(temp->side_destination) << ", " << side_to_char(side.second[i]->side_destination) << ", " << side_to_char(this->subgroup[destination][j]->side_destination) << std::endl;
                            this->subgroup[destination][j] = temp;
                            // std::cout << "3 step: " << side_to_char(temp->side_destination) << ", " << side_to_char(side.second[i]->side_destination) << ", " << side_to_char(this->subgroup[destination][j]->side_destination) << std::endl;
                            // std::cout << side.second[i] << " <-> " << this->subgroup[destination][j] << std::endl; 
                            // for(auto side : this->subgroup){
                            //     std::cout << side_to_char(side.first) << std::endl;
                            //     for(auto val : side.second){
                            //         std::cout << "[" << side_to_char(val->side_destination) << "]"; 
                            //     }
                            //     std::cout << std::endl;
                            // }
                            if(side.second[i]->side_destination == side.first){
                                this->no_of_misplaced_tiles--;
                            }
                            this->no_of_misplaced_tiles--;
                            break;
                        }
                    } // tiles swapped
                    // std::cout << "###############################" << std::endl;
                }
            }
        }
        if(old == this->no_of_misplaced_tiles){
            return 1;
        }
    }
    return 0;
}

int BasicCubeCentresSubgroup::optimal_solve(){
    /**
     *  Let's see the permutation as a directed graph where each side is a node 
     *  and we draw an arrow (directed edge) from the side with wrong tile
     *  to the side this tile belongs (its side_destination). On an edge we can save
     *  some more information about that tile, like where exactly it is situated on the cube.
     *  With this kind of graph prepared let's see that finding 2-node cycles is highly beneficial as
     *  each of 2-node cycles can be solved with only one commutator. Moreover finding more
     *  of the cycles is also wanted as solving along with the smallest cycle will alwyas give us 
     *  the smallest number of commutators needed.
     */
    return 0;
}

int BasicCubeCentresSubgroup::apply_to_cube(BasicCube* cube){
    for(auto swap : this->swap_path){
        // size_t row_index = 0;
        // size_t column_index = 0;
        // this->get_position(side, i, &row_index, &column_index);
        // this->convert_to_cube_index(side, row_index, column_index, &row_index, &column_index);

        auto moves = generate_commutator(swap.first.first, swap.first.second, swap.second.first, swap.second.second);
        for(auto move : moves){
            cube->solution_length++;
            cube->solution_path->push_back(move);
        }

        size_t first_row_index = 0;
        size_t second_row_index = 0;
        size_t first_column_index = 0;
        size_t second_column_index = 0;

        // std::cout << side_to_char(swap.first.first) << ", " << swap.first.second << " <-> " << side_to_char(swap.second.first) << ", " << swap.second.second << std::endl;


        this->get_position(swap.first.first, swap.first.second, &first_row_index, &first_column_index);
        this->convert_to_cube_index(swap.first.first, first_row_index, first_column_index, &first_row_index, &first_column_index);

        this->get_position(swap.second.first, swap.second.second, &second_row_index, &second_column_index);
        this->convert_to_cube_index(swap.second.first, second_row_index, second_column_index, &second_row_index, &second_column_index);
    
        // std::cout << first_row_index << ", " << first_column_index << " <-> " << second_row_index << ", " << second_column_index << std::endl;

        auto temp = (*cube->sides[swap.first.first])[first_row_index][first_column_index];
        (*cube->sides[swap.first.first])[first_row_index][first_column_index] = (*cube->sides[swap.second.first])[second_row_index][second_column_index];
        (*cube->sides[swap.second.first])[second_row_index][second_column_index] = temp;
        // cube->print_cube();
    }
    return 0;
}





