#include "../inc/thistlethwaite.hpp"
#include <array>
#include <utility>
#include <iostream>
#include <algorithm>

static inline std::map<std::pair<Side,Side>, bool>* initialize_phase1_map();

static inline bool check_move_sanity(Move* last_move, Move* new_move);

Thistlethwaite::Thistlethwaite(BasicCube* cube){
    Thistlethwaite::start_cube = cube;

    this->phase1_move_count = 0;
    this->phase2_move_count = 0;
    this->phase3_move_count = 0;
    this->phase4_move_count = 0;

    current_cubes = new std::queue<BasicCube*>();
    current_cubes->push(new BasicCube(cube));

    Move* move = new Move(Side::Left, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);

    move = new Move(Side::Left, 0, false, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);

    move = new Move(Side::Left, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);


    move = new Move(Side::Right, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);

    move = new Move(Side::Right, 0 , false, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);

    move = new Move(Side::Right, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);


    move = new Move(Side::Front, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);

    move = new Move(Side::Front, 0, false, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);

    move = new Move(Side::Front, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);


    move = new Move(Side::Back, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);

    move = new Move(Side::Back, 0, false, false);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);

    move = new Move(Side::Back, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);


    move = new Move(Side::Up, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);


    move = new Move(Side::Up, 0, false, false);
    Thistlethwaite::moves_p1.push_back(move);


    move = new Move(Side::Up, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);


    move = new Move(Side::Down, 0, true, false);
    Thistlethwaite::moves_p1.push_back(move);

    move = new Move(Side::Up, 0, false, false);
    Thistlethwaite::moves_p1.push_back(move);

    move = new Move(Side::Down, 0, true, true);
    Thistlethwaite::moves_p1.push_back(move);
    Thistlethwaite::moves_p2.push_back(move);
    Thistlethwaite::moves_p3.push_back(move);
    Thistlethwaite::moves_p4.push_back(move);

    Thistlethwaite::edge_orientation_map = initialize_phase1_map();

}

int Thistlethwaite::phase1_check(BasicCube* cube){
    int number = 0;

    // std::cout << "All map:" << std::endl;
    // for(auto key : *this->phase_1_map){
    //     std::cout << side_to_char(key.first.first) << ", " << side_to_char(key.first.second) << std::endl;
    // }
    // std::cout << "Found: " << std::endl;

    // check fu
    Side side1 = (*cube->side_front)[0][1].side_destination;
    Side side2 = (*cube->side_up)[CUBE_SIZE-1][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 1" << std::endl;
        number++;
    }

    // check fr
    side1 = (*cube->side_front)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_right)[1][0].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 2" << std::endl;
        number++;
    }

    // check fd
    side1 = (*cube->side_front)[CUBE_SIZE-1][1].side_destination;
    side2 = (*cube->side_down)[CUBE_SIZE-1][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 3" << std::endl;
        number++;
    }

    // check fl
    side1 = (*cube->side_front)[1][0].side_destination;
    side2 = (*cube->side_left)[1][CUBE_SIZE-1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 4" << std::endl;
        number++;
    }


    // check bu
    side1 = (*cube->side_back)[0][1].side_destination;
    side2 = (*cube->side_up)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 5" << std::endl;
        number++;
    }

    // check br
    side1 = (*cube->side_back)[1][0].side_destination;
    side2 = (*cube->side_right)[1][CUBE_SIZE-1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 6" << std::endl;
        number++;
    }

    // check bd
    side1 = (*cube->side_back)[CUBE_SIZE-1][1].side_destination;
    side2 = (*cube->side_down)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 7" << std::endl;
        number++;
    }

    // check bl
    side1 = (*cube->side_back)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_left)[1][0].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 8" << std::endl;
        number++;
    }


    // check ul
    side1 = (*cube->side_up)[1][0].side_destination;
    side2 = (*cube->side_left)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 9" << std::endl;
        number++;
    }

    // check ur
    side1 = (*cube->side_up)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_right)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 10" << std::endl;
        number++;
    }


    // check dl
    side1 = (*cube->side_down)[1][0].side_destination;
    side2 = (*cube->side_left)[CUBE_SIZE-1][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 11" << std::endl;
        number++;
    }

    // check dr
    side1 = (*cube->side_down)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_right)[CUBE_SIZE-1][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(this->edge_orientation_map->find(std::make_pair(side1, side2)) == this->edge_orientation_map->end()){
        // std::cout << "case 12" << std::endl;
        number++;
    }

    return number;
}

int Thistlethwaite::phase2_check(BasicCube* cube){
    int number = 0;

    Side side0 = (*cube->side_left)[0][0].side_destination;
    number += (side0 != Side::Left && side0 != Side::Right);
    side0 = (*cube->side_left)[0][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Left && side0 != Side::Right);
    side0 = (*cube->side_left)[CUBE_SIZE-1][0].side_destination;
    number += (side0 != Side::Left && side0 != Side::Right);
    side0 = (*cube->side_left)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Left && side0 != Side::Right);

    side0 = (*cube->side_right)[0][0].side_destination;
    number += (side0 != Side::Left && side0 != Side::Right);
    side0 = (*cube->side_right)[0][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Left && side0 != Side::Right);
    side0 = (*cube->side_right)[CUBE_SIZE-1][0].side_destination;
    number += (side0 != Side::Left && side0 != Side::Right);
    side0 = (*cube->side_right)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Left && side0 != Side::Right);

    Side side1 = (*cube->side_up)[CUBE_SIZE-1][1].side_destination;
    Side side2 = (*cube->side_front)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side1, side2)) == this->edge_slice_Y.end() &&
            std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side2, side1)) == this->edge_slice_Y.end()){
        number++;
        // std::cout << "case 1" << std::endl;
    }

    side1 = (*cube->side_front)[CUBE_SIZE-1][1].side_destination;
    side2 = (*cube->side_down)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side1, side2)) == this->edge_slice_Y.end() &&
            std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side2, side1)) == this->edge_slice_Y.end()){
        number++;
        // std::cout << "case 2" << std::endl;
    }

    side1 = (*cube->side_down)[CUBE_SIZE-1][1].side_destination;
    side2 = (*cube->side_back)[CUBE_SIZE-1][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side1, side2)) == this->edge_slice_Y.end() &&
            std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side2, side1)) == this->edge_slice_Y.end()){
        number++;
        // std::cout << "case 3" << std::endl;
    }

    side1 = (*cube->side_back)[0][1].side_destination;
    side2 = (*cube->side_up)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side1, side2)) == this->edge_slice_Y.end() &&
            std::find(this->edge_slice_Y.begin(), this->edge_slice_Y.end(), std::make_pair(side2, side1)) == this->edge_slice_Y.end()){
        number++;
        // std::cout << "case 4" << std::endl;
    }

    return number;
}

int Thistlethwaite::phase3_check(BasicCube* cube){
    int number = this->phase2_check(cube);

    Side side0 = (*cube->side_front)[0][0].side_destination;
    number += (side0 != Side::Front && side0 != Side::Back);
    side0 = (*cube->side_front)[0][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Front && side0 != Side::Back);
    side0 = (*cube->side_front)[CUBE_SIZE-1][0].side_destination;
    number += (side0 != Side::Front && side0 != Side::Back);
    side0 = (*cube->side_front)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Front && side0 != Side::Back);

    side0 = (*cube->side_back)[0][0].side_destination;
    number += (side0 != Side::Front && side0 != Side::Back);
    side0 = (*cube->side_back)[0][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Front && side0 != Side::Back);
    side0 = (*cube->side_back)[CUBE_SIZE-1][0].side_destination;
    number += (side0 != Side::Front && side0 != Side::Back);
    side0 = (*cube->side_back)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Front && side0 != Side::Back);

    Side side1 = (*cube->side_left)[1][CUBE_SIZE-1].side_destination;
    Side side2 = (*cube->side_front)[1][0].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side1, side2)) == this->edge_slice_X.end() &&
            std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side2, side1)) == this->edge_slice_X.end()){
        number++;
        // std::cout << "case 1" << std::endl;
    }

    side1 = (*cube->side_front)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_right)[1][0].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side1, side2)) == this->edge_slice_X.end() &&
            std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side2, side1)) == this->edge_slice_X.end()){
        number++;
        // std::cout << "case 2" << std::endl;
    }

    side1 = (*cube->side_right)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_back)[1][CUBE_SIZE-1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side1, side2)) == this->edge_slice_X.end() &&
            std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side2, side1)) == this->edge_slice_X.end()){
        number++;
        // std::cout << "case 3" << std::endl;
    }

    side1 = (*cube->side_back)[1][0].side_destination;
    side2 = (*cube->side_left)[1][0].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side1, side2)) == this->edge_slice_X.end() &&
            std::find(this->edge_slice_X.begin(), this->edge_slice_X.end(), std::make_pair(side2, side1)) == this->edge_slice_X.end()){
        number++;
        // std::cout << "case 4" << std::endl;
    }


    side0 = (*cube->side_up)[0][0].side_destination;
    number += (side0 != Side::Up && side0 != Side::Down);
    side0 = (*cube->side_up)[0][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Up && side0 != Side::Down);
    side0 = (*cube->side_up)[CUBE_SIZE-1][0].side_destination;
    number += (side0 != Side::Up && side0 != Side::Down);
    side0 = (*cube->side_up)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Up && side0 != Side::Down);

    side0 = (*cube->side_down)[0][0].side_destination;
    number += (side0 != Side::Up && side0 != Side::Down);
    side0 = (*cube->side_down)[0][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Up && side0 != Side::Down);
    side0 = (*cube->side_down)[CUBE_SIZE-1][0].side_destination;
    number += (side0 != Side::Up && side0 != Side::Down);
    side0 = (*cube->side_down)[CUBE_SIZE-1][CUBE_SIZE-1].side_destination;
    number += (side0 != Side::Up && side0 != Side::Down);

    side1 = (*cube->side_up)[1][CUBE_SIZE-1].side_destination;
    side2 = (*cube->side_right)[0][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side1, side2)) == this->edge_slice_Z.end() &&
            std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side2, side1)) == this->edge_slice_Z.end()){
        number++;
        // std::cout << "case 1" << std::endl;
    }

    side1 = (*cube->side_right)[CUBE_SIZE-1][1].side_destination;
    side2 = (*cube->side_down)[1][CUBE_SIZE-1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side1, side2)) == this->edge_slice_Z.end() &&
            std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side2, side1)) == this->edge_slice_Z.end()){
        number++;
        // std::cout << "case 2" << std::endl;
    }

    side1 = (*cube->side_down)[1][0].side_destination;
    side2 = (*cube->side_left)[CUBE_SIZE-1][1].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side1, side2)) == this->edge_slice_Z.end() &&
            std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side2, side1)) == this->edge_slice_Z.end()){
        number++;
        // std::cout << "case 3" << std::endl;
    }

    side1 = (*cube->side_left)[0][1].side_destination;
    side2 = (*cube->side_up)[1][0].side_destination;
    // std::cout << side_to_char(side1) << ", " << side_to_char(side2) << std::endl;
    if(std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side1, side2)) == this->edge_slice_Z.end() &&
            std::find(this->edge_slice_Z.begin(), this->edge_slice_Z.end(), std::make_pair(side2, side1)) == this->edge_slice_Z.end()){
        number++;
        // std::cout << "case 4" << std::endl;
    }

    return number;
}

int Thistlethwaite::phase4_check(BasicCube* cube){
    return cube->check_solution();
}

Thistlethwaite::~Thistlethwaite(){
    while(!current_cubes->empty()){
        BasicCube* cube = current_cubes->front();
        current_cubes->pop();
        delete cube;
    }
    delete current_cubes;
    
    for(auto move : moves_p1){
        delete move;
    }
    delete solved_cube;
    delete edge_orientation_map;
}

int Thistlethwaite::clear_current_cubes(){
    while(!this->current_cubes->empty()){
        BasicCube* cube = this->current_cubes->front();
        this->current_cubes->pop();
        delete cube;
    }
    return 0;
}

int Thistlethwaite::bfs(std::vector<Move*>* allowed_moves, int (Thistlethwaite::*check_function)(BasicCube*)){
    if(this->current_cubes->empty()){
        return -1;
    }
    
    BasicCube* cube = this->current_cubes->front();
    if(!(this->*check_function)(cube)){
        this->current_cubes->pop();
        this->clear_current_cubes();
        this->current_cubes->push(cube);
        return 0;
    }
    uint64_t max_depth = cube->solution_length;
    while(!this->current_cubes->empty()){
        cube = this->current_cubes->front();
        if(cube->solution_length > max_depth){
            max_depth = cube->solution_length;
            std::cout << "Current depth: " << max_depth << std::endl;
            // std::cout << "Something went wrong" << std::endl;
            // delete cube;
            // return -1;
        }
        this->current_cubes->pop();
        for(auto move : *allowed_moves){
            if(cube->solution_length > 0 &&
                !check_move_sanity(cube->solution_path->back(), move)){
                continue;
            }
            // std::cout << move->to_string() << std::endl;
            BasicCube* new_cube = new BasicCube(cube);
            new_cube->rotate(move);
            // new_cube->print_cube();
            // std::cout << (this->*check_function)(new_cube) << std::endl;
            if(!(this->*check_function)(new_cube)){
                this->clear_current_cubes();
                this->current_cubes->push(new_cube);
                delete cube;
                return 0;
            }
            this->current_cubes->push(new_cube);
        }
        delete cube;
    }
    return 0;
}

int Thistlethwaite::solve(){
    this->bfs(&moves_p1, &Thistlethwaite::phase1_check);
    std::cout << "End of phase 1" << std::endl;
    this->bfs(&moves_p2, &Thistlethwaite::phase2_check);
    std::cout << "End of phase 2" << std::endl;
    this->bfs(&moves_p3, &Thistlethwaite::phase3_check);
    std::cout << "End of phase 3" << std::endl;
    this->bfs(&moves_p4, &Thistlethwaite::phase4_check);
    std::cout << "End of phase 4" << std::endl;
    this->solved_cube = this->current_cubes->front();
    this->current_cubes->pop();
    return 0;
}

static inline std::map<std::pair<Side,Side>, bool>* initialize_phase1_map(){
    std::map<std::pair<Side,Side>, bool>* new_map = new std::map<std::pair<Side,Side>, bool>();
    new_map->insert({std::make_pair(Side::Front, Side::Up), true});
    new_map->insert({std::make_pair(Side::Front, Side::Left), true});
    new_map->insert({std::make_pair(Side::Front, Side::Right), true});
    new_map->insert({std::make_pair(Side::Front, Side::Down), true});
    
    new_map->insert({std::make_pair(Side::Back, Side::Up), true});
    new_map->insert({std::make_pair(Side::Back, Side::Left), true});
    new_map->insert({std::make_pair(Side::Back, Side::Right), true});
    new_map->insert({std::make_pair(Side::Back, Side::Down), true});

    new_map->insert({std::make_pair(Side::Up, Side::Left), true});
    new_map->insert({std::make_pair(Side::Up, Side::Right), true});

    new_map->insert({std::make_pair(Side::Down, Side::Left), true});
    new_map->insert({std::make_pair(Side::Down, Side::Right), true});

    return new_map;
}


static inline bool check_move_sanity(Move* last_move, Move* new_move){
    // Let's see that there is no need to move the same side twice, as:
    //  - two same quarter moves == half move - already calculated the step before
    //  - two same half moves == empty move - no need to go back to the step before
    //  - clockwise + counterclockwise move == empty move - 
    //                  no need to go back to the step before
    //  - quarter move + half move == quarter move of opposing clockwise
    if(last_move->side == new_move->side){
        return false;
    }


    // Also when two moves don't interfere with each other 
    // then their order doesn't matter.
    // This means we can restrict moves only to come up in specific order:
    //  - only F -> B, no Front move can occure directly after Back move
    //  - only L -> R, no Left move can occure directly after Right move
    //  - only U -> D, no Up move can occure directly after Down move
    if((last_move->side == Side::Back && new_move->side == Side::Front) ||
            (last_move->side == Side::Right && new_move->side == Side::Left) ||
            (last_move->side == Side::Down && new_move->side == Side::Up)){
        return false;
    }
    return true;
}
