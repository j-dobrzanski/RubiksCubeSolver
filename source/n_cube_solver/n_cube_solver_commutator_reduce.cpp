#include "../../inc/n_cube_solver/n_cube_solver.hpp"
#include "../../inc/basic_cube_subgroups/basic_cube_centres_subgroups.hpp"
#include "../../inc/basic_cube_subgroups/basic_cube_edges_subgroups.hpp"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <tuple>
#include <utility>

static int get_position_of_edge_sides(std::pair<Side, Side> edge, size_t index, size_t* row_index_side1, size_t* column_index_side1, size_t* row_index_side2, size_t* column_index_side2);

static int fix_corner_parity(BasicCube* cube);


static std::vector<Move*> generate_edge_pair_flip_commutator(std::pair<Side, Side> edge, size_t index);

std::array<std::pair<Side, Side>, 12> edge_orientation_set = {
    std::make_pair(Side::Front, Side::Left),
    std::make_pair(Side::Front, Side::Right),
    std::make_pair(Side::Front, Side::Up),
    std::make_pair(Side::Front, Side::Down),
    std::make_pair(Side::Back, Side::Left),
    std::make_pair(Side::Back, Side::Right),
    std::make_pair(Side::Back, Side::Up),
    std::make_pair(Side::Back, Side::Down),
    std::make_pair(Side::Up, Side::Left),
    std::make_pair(Side::Up, Side::Right),
    std::make_pair(Side::Down, Side::Left),
    std::make_pair(Side::Down, Side::Right)
};

std::map<std::pair<Side, Side>, std::pair<Side, Side>> commutator_map = {
    {std::make_pair(Side::Left, Side::Front), std::make_pair(Side::Up, Side::Down)},
    {std::make_pair(Side::Left, Side::Back), std::make_pair(Side::Down, Side::Up)},
    {std::make_pair(Side::Left, Side::Up), std::make_pair(Side::Back, Side::Front)},
    {std::make_pair(Side::Left, Side::Down), std::make_pair(Side::Front, Side::Back)},
    {std::make_pair(Side::Right, Side::Front), std::make_pair(Side::Down, Side::Up)},
    {std::make_pair(Side::Right, Side::Back), std::make_pair(Side::Up, Side::Down)},
    {std::make_pair(Side::Right, Side::Up), std::make_pair(Side::Front, Side::Back)},
    {std::make_pair(Side::Right, Side::Down), std::make_pair(Side::Back, Side::Front)},
    {std::make_pair(Side::Front, Side::Up), std::make_pair(Side::Left, Side::Right)},
    {std::make_pair(Side::Front, Side::Down), std::make_pair(Side::Right, Side::Left)},
    {std::make_pair(Side::Back, Side::Up), std::make_pair(Side::Right, Side::Left)},
    {std::make_pair(Side::Back, Side::Down), std::make_pair(Side::Left, Side::Right)},
};

int NCubeSolver::commutator_reduce(int option){
    (void) option;
    if(CUBE_SIZE == 3){
        this->reduced_cube = this->start_cube;
        return 0;
    }
    for(size_t i = 1; i < (CUBE_SIZE + 1) / 2; i++){
        for(size_t j = 1; j < CUBE_SIZE / 2; j++){
#ifndef NO_LOG
            std::cout << "Calculating subgroup for i,j: " << i << ", " << j << std::endl;
#endif
            BasicCubeCentresSubgroup* subgroup = new BasicCubeCentresSubgroup(this->start_cube, Side::Front, i, j);
            subgroup->naive_solve();
            // for(auto swap : subgroup->swap_path){
            //     std::cout << side_to_char(swap.first.first) << ", " << swap.first.second << " <-> " << side_to_char(swap.second.first) << ", " << swap.second.second << std::endl;
            // }
            subgroup->apply_to_cube(this->start_cube);
            // this->start_cube->print_cube();
            delete subgroup;
        }
    }

#ifndef NO_LOG
    start_cube->print_cube();
#endif


    for(size_t i = 1; i < (CUBE_SIZE + 1) / 2; i++){
        bool is_singular = false;
        if(i*2+1==CUBE_SIZE){
            is_singular = true;
        }
#ifndef NO_LOG
        std::cout << "Calculating subgroup for index: " << i << std::endl;
#endif
        BasicCubeEdgesSubgroup* subgroup = new BasicCubeEdgesSubgroup(this->start_cube, std::make_pair(Side::Front, Side::Up), i, is_singular); 
        subgroup->naive_solve();
        // subgroup->print_subgroup();
        subgroup->apply_to_cube(this->start_cube);
        delete subgroup;
    }

    // fix_corner_parity(this->start_cube);

#ifndef NO_LOG
    this->start_cube->print_cube();
#endif

    //* mapped to true if first edge part should be taken
    std::map<std::pair<Side, Side>, bool> chosen_orientation = std::map<std::pair<Side, Side>, bool>(); 
    std::map<std::pair<Side, Side>, std::array<std::array<size_t, 2>, 2>> edge_orientations = std::map<std::pair<Side, Side>, std::array<std::array<size_t, 2>, 2>>(); 
    size_t smallest_diff = CUBE_SIZE;
    std::pair<Side, Side> smallest_diff_edge;
    size_t orientation_sum = 0;
    bool flip_edge = false;


    for(auto edge : ordered_edge_set){
        auto cube_edge = this->start_cube->get_single_edge(edge.first, edge.second);
        auto base_edge_orientation = cube_edge[0];
        for(auto edge_part : cube_edge){
            if(base_edge_orientation == edge_part){
                edge_orientations[edge][0][0]++;
            }
            else{
                edge_orientations[edge][1][0]++;
            }
        }
        size_t diff = std::abs((int)edge_orientations[edge][0][0] - (int)edge_orientations[edge][1][0]);
        if(diff < smallest_diff){
            smallest_diff_edge = edge;
        }
        if((std::find(edge_orientation_set.begin(), edge_orientation_set.end(), edge) != edge_orientation_set.end()) ==
            (std::find(edge_orientation_set.begin(), edge_orientation_set.end(), cube_edge[0]) != edge_orientation_set.end())){
            edge_orientations[edge][0][1] = 0;
            edge_orientations[edge][1][1] = 1;
        }
        else{
            edge_orientations[edge][0][1] = 1;
            edge_orientations[edge][1][1] = 0;
        }
        if(edge_orientations[edge][0][0] > edge_orientations[edge][1][0]){
            orientation_sum += edge_orientations[edge][0][1];
        }
        else{
            orientation_sum += edge_orientations[edge][1][1];
        }
    }
    
    for(auto edge : ordered_edge_set){
        if(CUBE_SIZE % 2 == 0){
            chosen_orientation[edge] = edge_orientations[edge][0][0] > edge_orientations[edge][1][0];
        }
        else{
            auto cube_edge = this->start_cube->get_single_edge(edge.first, edge.second);
            if(cube_edge[0] == cube_edge[(EDGE_LENGTH - 1)/2]){
                // std::cout << side_to_char(cube_edge[0].first) << ", " << side_to_char(cube_edge[0].second) << " == " << side_to_char(cube_edge[(EDGE_LENGTH-1)/2].first) << ", " << side_to_char(cube_edge[(EDGE_LENGTH-1)/2].second) << "(" << (CUBE_SIZE - 1) /2 << ")"<< std::endl;
                chosen_orientation[edge] = 1;
            }
            else{
                // std::cout << side_to_char(cube_edge[0].first) << ", " << side_to_char(cube_edge[0].second) << " != " << side_to_char(cube_edge[(EDGE_LENGTH-1)/2].first) << ", " << side_to_char(cube_edge[(EDGE_LENGTH-1)/2].second) << "(" << (CUBE_SIZE - 1) /2 << ")" << std::endl;
                chosen_orientation[edge] = 0;
            }
        }
    }
    if(orientation_sum % 2 != 0 && CUBE_SIZE % 2 == 0){
        // calculate if its better to swap whole edge at the end or change oreintation for single edge parts
        chosen_orientation[smallest_diff_edge] = !chosen_orientation[smallest_diff_edge]; 

        // if((smallest_diff + 1) / 2 * 15 < ((CUBE_SIZE - 2) / 2)*7 + 8){
        //     chosen_orientation[smallest_diff_edge] = !chosen_orientation[smallest_diff_edge]; 
        // }
        // else{
        //     flip_edge = true;
        // }
#ifndef NO_LOG
        std::cout << "oreintation chnge" << std::endl;
#endif
    }

    // std::cout << "Edge orientations: " << std::endl;
    // for(auto edge : edge_orientations){
    //     std::cout << side_to_char(edge.first.first) << ", " << side_to_char(edge.first.second) << ": ";
    //     std::cout << "[" << edge.second[0][0] << ", " << edge.second[0][1] << "], [" << edge.second[1][0] << ", " << edge.second[1][1] << "] :: " << 1 - chosen_orientation[edge.first] << std::endl; 
    // }

    for(auto edge : ordered_edge_set){
        auto cube_edge = this->start_cube->get_single_edge(edge.first, edge.second);
        auto base_edge_orientation = cube_edge[0];
        // std::cout << side_to_char(base_edge_orientation.first) << ", " << side_to_char(base_edge_orientation.second) << std::endl;

        if(chosen_orientation[edge]){
            for(size_t i = 0; i < EDGE_LENGTH / 2; i++){
                if(cube_edge[i] != base_edge_orientation){
                    // std::cout << "Found edge part for swap" << std::endl; 
                    // std::cout << side_to_char(cube_edge[i].first) << ", " << side_to_char(cube_edge[i].second) << " : " << i << std::endl; 
                    // std::cout << side_to_char(base_edge_orientation.first) << ", " << side_to_char(base_edge_orientation.second) << std::endl;

                    auto commutator = generate_edge_pair_flip_commutator(edge, i+1);
                    for(auto move : commutator){
                        this->start_cube->solution_path->push_back(move);
                        this->start_cube->solution_length++;
                    }

                    size_t first_row_index_side1 = 0;
                    size_t first_row_index_side2 = 0;
                    size_t first_column_index_side1 = 0;
                    size_t first_column_index_side2 = 0;

                    size_t second_row_index_side1 = 0;
                    size_t second_row_index_side2 = 0;
                    size_t second_column_index_side1 = 0;
                    size_t second_column_index_side2 = 0;
            
                    get_position_of_edge_sides(edge, i + 1, &first_row_index_side1, &first_column_index_side1, &first_row_index_side2, &first_column_index_side2);
                    // std::cout << "\t" << side_to_char(edge.first) << ": " << first_row_index_side1 << ", " << first_column_index_side1 << std::endl; 
                    // std::cout << "\t" << side_to_char(edge.second) << ": " << first_row_index_side2 << ", " << first_column_index_side2 << std::endl; 


                    get_position_of_edge_sides(edge, CUBE_SIZE_LAST_INDEX - i - 1, &second_row_index_side1, &second_column_index_side1, &second_row_index_side2, &second_column_index_side2);
                    // std::cout << "\t" << side_to_char(edge.first) << ": " << second_row_index_side1 << ", " << second_column_index_side1 << std::endl; 
                    // std::cout << "\t" << side_to_char(edge.second) << ": " << second_row_index_side2 << ", " << second_column_index_side2 << std::endl; 

                    auto temp = (*this->start_cube->sides[edge.first])[first_row_index_side1][first_column_index_side1];
                    (*this->start_cube->sides[edge.first])[first_row_index_side1][first_column_index_side1] = (*this->start_cube->sides[edge.second])[second_row_index_side2][second_column_index_side2];
                    (*this->start_cube->sides[edge.second])[second_row_index_side2][second_column_index_side2] = temp;

                    temp = (*this->start_cube->sides[edge.first])[second_row_index_side1][second_column_index_side1];
                    (*this->start_cube->sides[edge.first])[second_row_index_side1][second_column_index_side1] = (*this->start_cube->sides[edge.second])[first_row_index_side2][first_column_index_side2];
                    (*this->start_cube->sides[edge.second])[first_row_index_side2][first_column_index_side2] = temp;

                    // this->start_cube->print_cube();
                }
            }
        }
        else{
            for(size_t i = 0; i < EDGE_LENGTH / 2; i++){
                if(cube_edge[i] == base_edge_orientation){
                    // std::cout << "Found edge part for swap" << std::endl; 
                    // std::cout << side_to_char(cube_edge[i].first) << ", " << side_to_char(cube_edge[i].second) << " : " << i << std::endl; 
                    // std::cout << side_to_char(base_edge_orientation.first) << ", " << side_to_char(base_edge_orientation.second) << std::endl;

                    auto commutator = generate_edge_pair_flip_commutator(edge, i+1);
                    for(auto move : commutator){
                        this->start_cube->solution_path->push_back(move);
                        this->start_cube->solution_length++;
                    }

                    size_t first_row_index_side1 = 0;
                    size_t first_row_index_side2 = 0;
                    size_t first_column_index_side1 = 0;
                    size_t first_column_index_side2 = 0;

                    size_t second_row_index_side1 = 0;
                    size_t second_row_index_side2 = 0;
                    size_t second_column_index_side1 = 0;
                    size_t second_column_index_side2 = 0;
                
                    get_position_of_edge_sides(edge, i + 1, &first_row_index_side1, &first_column_index_side1, &first_row_index_side2, &first_column_index_side2);
                    // std::cout << "\t" << side_to_char(edge.first) << ": " << first_row_index_side1 << ", " << first_column_index_side1 << std::endl; 
                    // std::cout << "\t" << side_to_char(edge.second) << ": " << first_row_index_side2 << ", " << first_column_index_side2 << std::endl; 


                    get_position_of_edge_sides(edge, CUBE_SIZE_LAST_INDEX - i - 1, &second_row_index_side1, &second_column_index_side1, &second_row_index_side2, &second_column_index_side2);
                    // std::cout << "\t" << side_to_char(edge.first) << ": " << second_row_index_side1 << ", " << second_column_index_side1 << std::endl; 
                    // std::cout << "\t" << side_to_char(edge.second) << ": " << second_row_index_side2 << ", " << second_column_index_side2 << std::endl; 

                    auto temp = (*this->start_cube->sides[edge.first])[first_row_index_side1][first_column_index_side1];
                    (*this->start_cube->sides[edge.first])[first_row_index_side1][first_column_index_side1] = (*this->start_cube->sides[edge.second])[second_row_index_side2][second_column_index_side2];
                    (*this->start_cube->sides[edge.second])[second_row_index_side2][second_column_index_side2] = temp;

                    temp = (*this->start_cube->sides[edge.first])[second_row_index_side1][second_column_index_side1];
                    (*this->start_cube->sides[edge.first])[second_row_index_side1][second_column_index_side1] = (*this->start_cube->sides[edge.second])[first_row_index_side2][first_column_index_side2];
                    (*this->start_cube->sides[edge.second])[first_row_index_side2][first_column_index_side2] = temp;


                    // this->start_cube->print_cube();
                }
            }
        }
    }

    if(flip_edge){
        for(auto edge : edge_orientations){
            if(edge.second[1 - chosen_orientation[edge.first]][1] == 1){
                // TODO add commutator
                for(size_t i = 1; i < CUBE_SIZE_LAST_INDEX; i++){ // a bit cheesy swap
                    size_t row_index_side1 = 0;
                    size_t row_index_side2 = 0;
                    size_t column_index_side1 = 0;
                    size_t column_index_side2 = 0;

                    get_position_of_edge_sides(edge.first, i, &row_index_side1, &column_index_side1, &row_index_side2, &column_index_side2);

                    auto temp = (*this->start_cube->sides[edge.first.first])[row_index_side1][column_index_side1];
                    (*this->start_cube->sides[edge.first.first])[row_index_side1][column_index_side1] = (*this->start_cube->sides[edge.first.second])[row_index_side2][column_index_side2];
                    (*this->start_cube->sides[edge.first.second])[row_index_side2][column_index_side2] = temp;
                }
                break;
            }
        }
    }

    fix_corner_parity(this->start_cube);

    this->reduced_cube = this->start_cube;
    return 0;
}


static int get_position_of_edge_sides(std::pair<Side, Side> edge, size_t index, size_t* row_index_side1, size_t* column_index_side1, size_t* row_index_side2, size_t* column_index_side2){
    if(compare_edge(edge, std::make_pair(Side::Left, Side::Front))){
        *row_index_side1 = index;
        *column_index_side1 = CUBE_SIZE_LAST_INDEX;
        *row_index_side2 = index;
        *column_index_side2 = 0;
        return 0;

    }
    else if(compare_edge(edge, std::make_pair(Side::Left, Side::Back))){
        *row_index_side1 = index;
        *column_index_side1 = 0;
        *row_index_side2 = index;
        *column_index_side2 = 0;
        return 0;
    }
    else if(compare_edge(edge, std::make_pair(Side::Left, Side::Up))){
        *row_index_side1 = 0;
        *column_index_side1 = index;
        *row_index_side2 = index;
        *column_index_side2 = 0;
        return 0;
    }
    else if(compare_edge(edge, std::make_pair(Side::Left, Side::Down))){
        *row_index_side1 = CUBE_SIZE_LAST_INDEX;
        *column_index_side1 = index;
        *row_index_side2 = index;
        *column_index_side2 = 0;
        return 0;
    }
    else if(compare_edge(edge, std::make_pair(Side::Right, Side::Front))){
        *row_index_side1 = index;
        *column_index_side1 = 0;
        *row_index_side2 = index;
        *column_index_side2 = CUBE_SIZE_LAST_INDEX;
        return 0;
    }
    else if(compare_edge(edge, std::make_pair(Side::Right, Side::Back))){
        *row_index_side1 = index;
        *column_index_side1 = CUBE_SIZE_LAST_INDEX;
        *row_index_side2 = index;
        *column_index_side2 = CUBE_SIZE_LAST_INDEX;
        return 0;
    }
    else if(compare_edge(edge, std::make_pair(Side::Right, Side::Up))){
        *row_index_side1 = 0;
        *column_index_side1 = index;
        *row_index_side2 = CUBE_SIZE_LAST_INDEX - index;
        *column_index_side2 = CUBE_SIZE_LAST_INDEX;
        return 0;
    }
    else if(compare_edge(edge, std::make_pair(Side::Right, Side::Down))){
        *row_index_side1 = CUBE_SIZE_LAST_INDEX;
        *column_index_side1 = index;
        *row_index_side2 = CUBE_SIZE_LAST_INDEX - index;
        *column_index_side2 = CUBE_SIZE_LAST_INDEX;
        return 0;
    }
    else if(compare_edge(edge, std::make_pair(Side::Front, Side::Up))){
        *row_index_side1 = 0;
        *column_index_side1 = index;
        *row_index_side2 = CUBE_SIZE_LAST_INDEX;
        *column_index_side2 = index;
        return 0;
    }
    else if(compare_edge(edge, std::make_pair(Side::Front, Side::Down))){
        *row_index_side1 = CUBE_SIZE_LAST_INDEX;
        *column_index_side1 = index;
        *row_index_side2 = CUBE_SIZE_LAST_INDEX;
        *column_index_side2 = index;
        return 0;
    }
    else if(compare_edge(edge, std::make_pair(Side::Back, Side::Up))){
        *row_index_side1 = 0;
        *column_index_side1 = index;
        *row_index_side2 = 0;
        *column_index_side2 = index;
        return 0;
    }
    else if(compare_edge(edge, std::make_pair(Side::Back, Side::Down))){
        *row_index_side1 = CUBE_SIZE_LAST_INDEX;
        *column_index_side1 = index;
        *row_index_side2 = 0;
        *column_index_side2 = index;
        return 0;
    }
    return 1;
}

static std::vector<Move*> generate_edge_pair_flip_commutator(std::pair<Side, Side> edge, size_t index){
    std::vector<Move*> moves = std::vector<Move*>();
    edge = normalize_edge(edge);

    // in original commutator fo FU we use Front/Up and Left/Right moves, but here we need to be flexible
    Side pseudo_right = commutator_map[edge].first;
    Side pseudo_left = commutator_map[edge].second;
    Side pseudo_front = edge.first;
    Side pseudo_up = edge.second;

    moves.push_back(new Move(pseudo_right, index, false, false));
    moves.push_back(new Move(pseudo_up, 0, true, true));
    moves.push_back(new Move(pseudo_left, index, true, true));
    moves.push_back(new Move(pseudo_front, 0, true, true));
    moves.push_back(new Move(pseudo_left, index, false, false));
    moves.push_back(new Move(pseudo_front, 0, true, true));    
    moves.push_back(new Move(pseudo_right, index, true, true));
    moves.push_back(new Move(pseudo_up, 0, true, true));
    moves.push_back(new Move(pseudo_right, index, true, false));
    moves.push_back(new Move(pseudo_up, 0, true, true));
    moves.push_back(new Move(pseudo_right, index, false, false));
    moves.push_back(new Move(pseudo_up, 0, true, true));
    moves.push_back(new Move(pseudo_front, 0, true, true));
    moves.push_back(new Move(pseudo_right, index, true, false));
    moves.push_back(new Move(pseudo_front, 0, true, true));


    return moves;
}


static int fix_corner_parity(BasicCube* cube){
    std::map<std::tuple<Side, Side, Side>, std::tuple<Side, Side, Side>> corner_map = std::map<std::tuple<Side, Side, Side>, std::tuple<Side, Side, Side>>();
    std::map<std::tuple<Side, Side, Side>, bool> visited_map = std::map<std::tuple<Side, Side, Side>, bool>();
    for(Side side0 : {Side::Left, Side::Right}){
        for(Side side1 : {Side::Front, Side::Back}){
            for(Side side2 : {Side::Up, Side::Down}){
                corner_map[std::make_tuple(side0, side1, side2)] = normalize_corner(cube->get_corner(side0, side2, side1));
                visited_map[std::make_tuple(side0, side1, side2)] = false;
            }
        }
    }
    size_t no_of_type1 = 0;
    // size_t no_of_type2 = 0;
    std::pair<std::tuple<Side, Side, Side>, std::tuple<Side, Side, Side>> last_type1_pair;
    // std::pair<std::tuple<Side, Side, Side>, std::tuple<Side, Side, Side>> last_type2_pair;
    // size_t no_of_visited = 0;
    for(Side side0 : {Side::Left, Side::Right}){
        for(Side side1 : {Side::Front, Side::Back}){
            for(Side side2 : {Side::Up, Side::Down}){
                if(!visited_map[std::make_tuple(side0, side1, side2)]){
                    std::tuple<Side, Side, Side> start_corner = std::make_tuple(side0, side1, side2);
                    std::tuple<Side, Side, Side> curent_corner = start_corner;
                    std::tuple<Side, Side, Side> next_corner = corner_map[curent_corner];
                    visited_map[start_corner];
                    // std::cout << "(" << side_to_char(side0) << ", " << side_to_char(side1) << ", " << side_to_char(side2) << ") ";
                    while(start_corner != next_corner){
                        // std::cout << "-> (" << side_to_char(std::get<0>(next_corner)) << ", " << side_to_char(std::get<1>(next_corner)) << ", " << side_to_char(std::get<2>(next_corner)) << ") ";
                        visited_map[next_corner] = true;
                        // size_t diff = 0;
                        // if(std::get<0>(curent_corner) != std::get<0>(next_corner)){
                        //     diff++;
                        // }
                        // if(std::get<1>(curent_corner) != std::get<1>(next_corner)){
                        //     diff++;
                        // }
                        // if(std::get<2>(curent_corner) != std::get<2>(next_corner)){
                        //     diff++;
                        // }
                        // if(diff == 1){
                        //     no_of_type1++;
                        //     last_type1_pair = std::make_pair(curent_corner, next_corner);
                        // }
                        // else if(diff == 2){
                        //     no_of_type2++;
                        //     last_type2_pair = std::make_pair(curent_corner, next_corner);
                        // }
                        // else if(diff == 3){
                        //     no_of_type1++;
                        //     no_of_type2 += 2;

                        // }
                        // else{
                        //     std::cout << "Error" << __LINE__ << std::endl;
                        //     return -1;
                        // }
                        no_of_type1++;
                        curent_corner = next_corner;
                        next_corner = corner_map[curent_corner];
                    }
                    // std::cout << std::endl;
                }
            }
        }
    }

    if(no_of_type1 % 2 == 1){
        // we need to flip
#ifndef NO_LOG
        std::cout << "flip!" << std::endl;
#endif
        // adding commutator to path
        Move* move;

        move = new Move(Side::Right, 0, true, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Up, 0, true, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Right, 0, false, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Up, 0, false, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Right, 0, false, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Front, 0, true, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Right, 0, true, true);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Up, 0, false, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Right, 0, false, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Up, 0, false, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Right, 0, true, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Up, 0, true, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Right, 0, false, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        move = new Move(Side::Front, 0, false, false);
        cube->solution_path->push_back(move);
        cube->solution_length++;

        // updte the cube
        BasicCubeTile temp_corner = (*cube->side_up)[0][CUBE_SIZE_LAST_INDEX];
        (*cube->side_up)[0][CUBE_SIZE_LAST_INDEX] = (*cube->side_up)[CUBE_SIZE_LAST_INDEX][CUBE_SIZE_LAST_INDEX];
        (*cube->side_up)[CUBE_SIZE_LAST_INDEX][CUBE_SIZE_LAST_INDEX] = temp_corner;

        temp_corner = (*cube->side_front)[0][CUBE_SIZE_LAST_INDEX];
        (*cube->side_front)[0][CUBE_SIZE_LAST_INDEX] = (*cube->side_right)[0][CUBE_SIZE_LAST_INDEX];
        (*cube->side_right)[0][CUBE_SIZE_LAST_INDEX] = temp_corner;

        temp_corner = (*cube->side_right)[0][0];
        (*cube->side_right)[0][0] = (*cube->side_back)[0][CUBE_SIZE_LAST_INDEX];
        (*cube->side_back)[0][CUBE_SIZE_LAST_INDEX] = temp_corner;

    }


    // for(size_t i = 0; i < EDGE_LENGTH; i++){
    //     BasicCubeTile temp_edge1 = (*cube->side_left)[0][i + 1];
    //     BasicCubeTile temp_edge2 = (*cube->side_up)[i + 1][0];
    //     (*cube->side_left)[0][i + 1] = (*cube->side_right)[0][i + 1];
    //     (*cube->side_up)[i + 1][0] = (*cube->side_up)[i + 1][CUBE_SIZE_LAST_INDEX];
    //     (*cube->side_right)[0][i + 1] = temp_edge1;
    //     (*cube->side_up)[i + 1][CUBE_SIZE_LAST_INDEX] = temp_edge2;
    // }

    return 0;
}
