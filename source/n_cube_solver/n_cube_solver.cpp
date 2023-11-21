#include "../../inc/n_cube_solver/n_cube_solver.hpp"
#include <cstddef>
#include <vector>

static int generate_all_possible_moves(std::vector<Move*>* moves);

NCubeSolver::NCubeSolver(BasicCube* cube){
    this->start_cube = new BasicCube(cube);
    start_cube->clear_solution_path();
    this->solved_cube = NULL;
    this->in_between_steps_cube = NULL;
    this->current_cubes = new std::priority_queue<BasicCube*, std::vector<BasicCube*>, Compare_cubes>();
    this->current_cubes->push(new BasicCube(start_cube));

    this->all_possible_moves = std::vector<Move*>();

    generate_all_possible_moves(&this->all_possible_moves);
}

int NCubeSolver::solve(){
    
    return 0;
}


NCubeSolver::~NCubeSolver(){
    delete start_cube;

    if(solved_cube != NULL){
        delete solved_cube;
    }

    while(!current_cubes->empty()){
        BasicCube* cube = current_cubes->top();
        current_cubes->pop();
        delete cube;
    }

    delete current_cubes;

    for(auto move : this->all_possible_moves){
        delete move;
    }

    if(this->in_between_steps_cube != NULL){
        delete this->in_between_steps_cube;
    }

}

static int generate_all_possible_moves(std::vector<Move*>* moves){
    Side sides[6] = {
        Side::Left,
        Side::Right,
        Side::Front,
        Side::Back,
        Side::Up,
        Side::Down
    };
    for(Side side : sides){
        for(size_t i = 0; i < CUBE_SIZE / 2; i++){
            moves->push_back(new Move(side, (uint8_t)i, true, false));
            moves->push_back(new Move(side, (uint8_t)i, false, false));
            moves->push_back(new Move(side, (uint8_t)i, true, true));
        }
    }
    return 0;
}
