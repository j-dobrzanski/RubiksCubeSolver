#include "../../inc/n_cube_solver/n_cube_solver.hpp"
#include <iostream>

static size_t side_center_solved(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side, Side destination_side);
static size_t edge_solved(std::array<std::pair<Side, Side>, EDGE_LENGTH> edge, std::pair<Side, Side> destination_edge);

size_t NCubeSolver::check_step_1_completion(BasicCube* cube){
    size_t conditions = side_center_solved(cube->sides[this->opposite_sides_step_1.first],
                                         this->opposite_sides_step_1.first) +
                        side_center_solved(cube->sides[this->opposite_sides_step_1.second],
                                           this->opposite_sides_step_1.second);
    return conditions;
}

size_t NCubeSolver::check_step_2_completion(BasicCube* cube){
    size_t conditions = this->check_step_1_completion(cube) +
                        side_center_solved(cube->sides[this->adjacent_sides_step_2.first],
                                           this->adjacent_sides_step_2.first) +
                        side_center_solved(cube->sides[this->adjacent_sides_step_2.second],
                                           this->adjacent_sides_step_2.second);
    return conditions;
}

size_t NCubeSolver::check_step_3_completion(BasicCube* cube){
    size_t conditions = this->check_step_2_completion(cube) +
                        side_center_solved(cube->sides[this->last_sides_step_3.first],
                                           this->last_sides_step_3.first) +
                        side_center_solved(cube->sides[this->last_sides_step_3.second],
                                           this->last_sides_step_3.second);
    return conditions;
}

size_t NCubeSolver::check_step_4_completion(BasicCube* cube){
    size_t conditions = this->check_step_3_completion(cube) +
                        edge_solved(cube->get_edge_FD(), this->first_4_edges_step_4[0]) +
                        edge_solved(cube->get_edge_RD(), this->first_4_edges_step_4[1]) +
                        edge_solved(cube->get_edge_BD(), this->first_4_edges_step_4[2]) +
                        edge_solved(cube->get_edge_LD(), this->first_4_edges_step_4[3]);

    return conditions;
}

size_t NCubeSolver::check_step_5_completion(BasicCube* cube){
    size_t conditions = this->check_step_4_completion(cube) +
                        edge_solved(cube->get_edge_FU(), this->second_4_edges_step_5[0]) +
                        edge_solved(cube->get_edge_RU(), this->second_4_edges_step_5[1]) +
                        edge_solved(cube->get_edge_BU(), this->second_4_edges_step_5[2]) +
                        edge_solved(cube->get_edge_LU(), this->second_4_edges_step_5[3]);

    return conditions;
}

size_t NCubeSolver::check_step_6_completion(BasicCube* cube){
    size_t conditions = this->check_step_4_completion(cube) +
                        edge_solved(cube->get_edge_LF(), this->last_4_edges_step_6[0]) +
                        edge_solved(cube->get_edge_RF(), this->last_4_edges_step_6[1]) +
                        edge_solved(cube->get_edge_RB(), this->last_4_edges_step_6[2]) +
                        edge_solved(cube->get_edge_LB(), this->last_4_edges_step_6[3]);

    return conditions;
}

static size_t side_center_solved(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side, Side destination_side){
    size_t counter = 0;
    // std::cout << "side: " << side_to_char(destination_side) << std::endl;
    for(size_t i = 1; i < CUBE_SIZE_LAST_INDEX; i++){
        for(size_t j = 1; j < CUBE_SIZE_LAST_INDEX; j++){
            if((*side)[i][j].side_destination != destination_side){
                counter++;
            }
            // else{
            //     std::cout << "(*side)[" << i << "][" << j << "].side_destination = " << side_to_char((*side)[i][j].side_destination) << std::endl;
            // }
        }
    }
    return counter;    
}

static size_t edge_solved(std::array<std::pair<Side, Side>, EDGE_LENGTH> edge, std::pair<Side, Side> destination_edge){
    size_t counter = 0;
    for(auto edge_part : edge){
        if(edge_part != destination_edge){
            counter++;
        }
    }
    return counter;
}

