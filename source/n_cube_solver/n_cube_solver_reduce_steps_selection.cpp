#include "../../inc/n_cube_solver/n_cube_solver.hpp"
#include <array>
#include <map>
#include <utility>
#include <iostream>

static size_t count_tiles_on_side_center(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* cube_side, Side side);
static std::map<std::pair<Side, Side>, size_t> count_tiles_on_edge(std::array<std::pair<Side, Side>, EDGE_LENGTH> edge);
static std::pair<size_t, size_t> all_edge_parts_different(std::array<std::pair<Side, Side>, 4> edge_set);
static std::pair<Side, Side> extract_best_edge_part_from_map(std::map<std::pair<Side, Side>, size_t>* edge_map, size_t* edge_value);
static std::array<std::pair<Side, Side>, 4> choose_edges_from_maps(std::map<std::pair<Side, Side>, size_t> maps[]);

std::pair<Side, Side> NCubeSolver::choose_opposite_sides_step_1(BasicCube* cube){
    // count sum of solved tiles for LEFT/Right pair of opposing sides:
    size_t left_right_sum = count_tiles_on_side_center(cube->side_left, Side::Left) + 
                                count_tiles_on_side_center(cube->side_right, Side::Right);

    size_t front_back_sum = count_tiles_on_side_center(cube->side_front, Side::Front) + 
                                count_tiles_on_side_center(cube->side_back, Side::Back);

    size_t up_down_sum = count_tiles_on_side_center(cube->side_up, Side::Up) + 
                                count_tiles_on_side_center(cube->side_down, Side::Down);
    if(left_right_sum > front_back_sum){
        if(left_right_sum > up_down_sum){
            return std::make_pair(Side::Left, Side::Right);
        }
        else{
            return std::make_pair(Side::Up, Side::Down);
        }
    }
    else{
        if(front_back_sum > up_down_sum){
            return std::make_pair(Side::Front, Side::Back);
        }
        else{
            return std::make_pair(Side::Up, Side::Down);
        }
    }
}


std::pair<Side, Side> NCubeSolver::choose_adjacent_sides_step_2(BasicCube* cube){
    if(this->opposite_sides_step_1 == std::make_pair(Side::Left, Side::Right)){
        size_t front_sum = count_tiles_on_side_center(cube->side_front, Side::Front);
        size_t down_sum = count_tiles_on_side_center(cube->side_down, Side::Down);
        size_t back_sum = count_tiles_on_side_center(cube->side_back, Side::Back);
        size_t up_sum = count_tiles_on_side_center(cube->side_up, Side::Up);
        if(front_sum > back_sum){
            if(up_sum > down_sum){
                return std::make_pair(Side::Front, Side::Up);
            }
            else{
                return std::make_pair(Side::Front, Side::Down);
            }
        }
        else{
            if(up_sum > down_sum){
                return std::make_pair(Side::Back, Side::Up);
            }
            else{
                return std::make_pair(Side::Back, Side::Down);
            }
        }
    }
    else if(this->opposite_sides_step_1 == std::make_pair(Side::Front, Side::Back)){
        size_t left_sum = count_tiles_on_side_center(cube->side_left, Side::Left);
        size_t down_sum = count_tiles_on_side_center(cube->side_down, Side::Down);
        size_t right_sum = count_tiles_on_side_center(cube->side_right, Side::Right);
        size_t up_sum = count_tiles_on_side_center(cube->side_up, Side::Up);
        if(left_sum > right_sum){
            if(up_sum > down_sum){
                return std::make_pair(Side::Left, Side::Up);
            }
            else{
                return std::make_pair(Side::Left, Side::Down);
            }
        }
        else{
            if(up_sum > down_sum){
                return std::make_pair(Side::Right, Side::Up);
            }
            else{
                return std::make_pair(Side::Right, Side::Down);
            }
        }
    }
    else{
        size_t front_sum = count_tiles_on_side_center(cube->side_front, Side::Front);
        size_t right_sum = count_tiles_on_side_center(cube->side_right, Side::Right);
        size_t back_sum = count_tiles_on_side_center(cube->side_back, Side::Back);
        size_t left_sum = count_tiles_on_side_center(cube->side_left, Side::Left);
        if(left_sum > right_sum){
            if(front_sum > back_sum){
                return std::make_pair(Side::Left, Side::Front);
            }
            else{
                return std::make_pair(Side::Left, Side::Back);
            }
        }
        else{
            if(front_sum > back_sum){
                return std::make_pair(Side::Right, Side::Front);
            }
            else{
                return std::make_pair(Side::Right, Side::Back);
            }
        }
    }
}


std::pair<Side, Side> NCubeSolver::choose_last_sides_step_3(BasicCube* cube){
    (void) cube;
    Side sides[2];
    std::map<Side, bool> side_map = {
        {Side::Left, false},
        {Side::Right, false},
        {Side::Front, false},
        {Side::Back, false},
        {Side::Up, false},
        {Side::Down, false}
    };
    side_map[std::get<0>(this->opposite_sides_step_1)] = true;
    side_map[std::get<1>(this->opposite_sides_step_1)] = true;

    side_map[std::get<0>(this->adjacent_sides_step_2)] = true;
    side_map[std::get<1>(this->adjacent_sides_step_2)] = true;

    size_t counter = 0;
    for(auto key : side_map){
        if(!std::get<1>(key)){
            if(counter > 1){
                std::cout << "ERROR     Wrong sides chosen. l:" << __LINE__ << std::endl;
            }
            sides[counter] = std::get<0>(key);
            counter++;
        }
    }
    return std::make_pair(sides[0], sides[1]);
}


std::array<std::pair<Side, Side>, 4> NCubeSolver::choose_first_4_edges_step_4(BasicCube* cube){
    std::map<std::pair<Side, Side>, size_t> maps[4] = {
        count_tiles_on_edge(cube->get_edge_FD()),
        count_tiles_on_edge(cube->get_edge_RD()),
        count_tiles_on_edge(cube->get_edge_BD()),
        count_tiles_on_edge(cube->get_edge_LD())
    };

    return choose_edges_from_maps(maps);
}


std::array<std::pair<Side, Side>, 4> NCubeSolver::choose_second_4_edges_step_5(BasicCube* cube){
    std::map<std::pair<Side, Side>, size_t> maps[4] = {
        count_tiles_on_edge(cube->get_edge_FU()),
        count_tiles_on_edge(cube->get_edge_RU()),
        count_tiles_on_edge(cube->get_edge_BU()),
        count_tiles_on_edge(cube->get_edge_LU())
    };

    return choose_edges_from_maps(maps);
}


std::array<std::pair<Side, Side>, 4> NCubeSolver::choose_last_4_edges_step_6(BasicCube* cube){
    std::map<std::pair<Side, Side>, size_t> maps[4] = {
        count_tiles_on_edge(cube->get_edge_LF()),
        count_tiles_on_edge(cube->get_edge_RF()),
        count_tiles_on_edge(cube->get_edge_RB()),
        count_tiles_on_edge(cube->get_edge_LB())
    };

    return choose_edges_from_maps(maps);
}


static size_t count_tiles_on_side_center(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* cube_side, Side side){
    size_t count = 0;
    for(size_t i = 1; i < CUBE_SIZE_LAST_INDEX - 1; i++){
        for(size_t j = 1; j < CUBE_SIZE_LAST_INDEX - 1; j++){
            count += (*cube_side)[i][j].side_destination == side;
        }
    }
    return count;
}

static std::map<std::pair<Side, Side>, size_t> count_tiles_on_edge(std::array<std::pair<Side, Side>, EDGE_LENGTH> edge){
    std::map<std::pair<Side, Side>, size_t> count_map = std::map<std::pair<Side, Side>, size_t>();
    for(auto edge_part : unordered_edge_set){
        count_map[edge_part] = 0;
    }
    for(auto edge_part : edge){
        if(count_map.count(edge_part) == 1){
            count_map[edge_part] += 1;
        }
    }
    return count_map;
}

static std::pair<size_t, size_t> all_edge_parts_different(std::array<std::pair<Side, Side>, 4> edge_set){
    for(size_t i = 0; i < 4; i++){
        for(size_t j = i + 1; j < 4; j++){
            if(compare_edge(edge_set[i], edge_set[j])){
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(0, 0);
}

static std::pair<Side, Side> extract_best_edge_part_from_map(std::map<std::pair<Side, Side>, size_t>* edge_map, size_t* edge_value){
    size_t temp = 0;
    std::pair<Side, Side> edge;
    for(auto edge_part : unordered_edge_set){
        if((*edge_map)[edge_part] > temp){
            temp = (*edge_map)[edge_part];
            edge = edge_part;
        }
    }
    *edge_value = temp;
    (*edge_map)[edge] = 0;
    return edge;
}

static std::array<std::pair<Side, Side>, 4> choose_edges_from_maps(std::map<std::pair<Side, Side>, size_t> maps[]){
    size_t values[4] = {0};

    std::array<std::pair<Side, Side>, 4> chosen_edges = {
        extract_best_edge_part_from_map(&(maps[0]), &(values[0])),
        extract_best_edge_part_from_map(&(maps[1]), &(values[1])),
        extract_best_edge_part_from_map(&(maps[2]), &(values[2])),
        extract_best_edge_part_from_map(&(maps[3]), &(values[3]))
    };
    
    std::pair<size_t, size_t> collisions = all_edge_parts_different(chosen_edges);
    while(collisions != std::make_pair((size_t)0, (size_t)0)){
        if(values[collisions.first] > values[collisions.second]){
            chosen_edges[collisions.second] = extract_best_edge_part_from_map(&(maps[collisions.second]), &(values[collisions.second]));
        }
        else{
            chosen_edges[collisions.first] = extract_best_edge_part_from_map(&(maps[collisions.first]), &(values[collisions.first]));
        }
        collisions = all_edge_parts_different(chosen_edges);
    }
    return chosen_edges;
}

