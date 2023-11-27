#include "../../inc/n_cube_solver/n_cube_solver.hpp"
#include <queue>
#include <stack>
#include <iostream>
#include <vector>

int NCubeSolver::reduce(){
    std::cout << "selection for step 1" << std::endl;
    this->opposite_sides_step_1 = this->choose_opposite_sides_step_1(this->start_cube);

    // here do iddfs to solve this step
    std::cout << "iddfs for step 1" << std::endl;
    if(this->reduce_iddfs(&NCubeSolver::check_step_1_completion, this->start_cube) != 0){
        return 1;
    }

    this->in_between_steps_cube->print_cube();

    std::cout << "selection for step 2" << std::endl;
    this->adjacent_sides_step_2 = this->choose_adjacent_sides_step_2(this->in_between_steps_cube);

    // here do iddfs to solve this step
    std::cout << "iddfs for step 2" << std::endl;
    if(this->reduce_iddfs(&NCubeSolver::check_step_2_completion, this->in_between_steps_cube) != 0){
        return 1;
    }

    this->in_between_steps_cube->print_cube();

    std::cout << "selection for step 3" << std::endl;
    this->last_sides_step_3 = this->choose_last_sides_step_3(this->in_between_steps_cube);

    // here do iddfs to solve this step
    std::cout << "iddfs for step 3" << std::endl;
    if(this->reduce_iddfs(&NCubeSolver::check_step_3_completion, this->in_between_steps_cube)!= 0){
        return 1;
    }

    this->in_between_steps_cube->print_cube();

    std::cout << "selection for step 4" << std::endl;
    this->first_4_edges_step_4 = this->choose_first_4_edges_step_4(this->in_between_steps_cube);

    // here do iddfs to solve this step
    std::cout << "iddfs for step 4" << std::endl;
    if(this->reduce_iddfs(&NCubeSolver::check_step_4_completion, this->in_between_steps_cube)!= 0){
        return 1;
    }

    this->in_between_steps_cube->print_cube();

    std::cout << "selection for step 5" << std::endl;
    this->second_4_edges_step_5 = this->choose_second_4_edges_step_5(this->in_between_steps_cube);

    // here do iddfs to solve this step
    std::cout << "iddfs for step 5" << std::endl;
    if(this->reduce_iddfs(&NCubeSolver::check_step_5_completion, this->in_between_steps_cube)!= 0){
        return 1;
    }

    this->in_between_steps_cube->print_cube();

    std::cout << "selection for step 6" << std::endl;
    this->last_4_edges_step_6 = this->choose_last_4_edges_step_6(this->in_between_steps_cube);

    // here do iddfs to solve this step
    std::cout << "iddfs for step 6" << std::endl;
    if(this->reduce_iddfs(&NCubeSolver::check_step_6_completion, this->in_between_steps_cube)!= 0){
        return 1;
    }

    this->reduced_cube = this->in_between_steps_cube;
    return 0;
}

int NCubeSolver::reduce_iddfs(size_t (NCubeSolver::*check_step_completion)(BasicCube*), BasicCube* step_start_cube){
    if(!(this->*check_step_completion)(step_start_cube)){
        this->in_between_steps_cube = step_start_cube;
        return 0;
    }

    // size_t depth = 1;
    BasicCube* result_cube = {0};
    int result = -1;

    // while(result != 0/* && depth < 25*/){ // throw away depth max later
    //     std::cout << "Currently performing dfs with max depth: " << depth << std::endl;
    //     result = this->reduce_single_dfs(check_step_completion, step_start_cube, depth, &result_cube);
    //     depth++;
    // }

    result = this->reduce_single_dfs(check_step_completion, step_start_cube, 100, &result_cube);

    if(result != 0){
        std::cout << "ERROR:   SOLUTION PATH TO CHECKPOINT NOT FOUND" << std::endl;
        return -1;
    }
    if(this->in_between_steps_cube != NULL){
        delete this->in_between_steps_cube;
    }
    this->in_between_steps_cube = result_cube;
    return 0;
}

int NCubeSolver::reduce_single_dfs(size_t (NCubeSolver::*check_step_completion)(BasicCube*), BasicCube* step_start_cube, size_t max_depth, BasicCube** solved_cube){
    std::priority_queue<BasicCube*, std::vector<BasicCube*>, Compare_cubes> current_cubes = std::priority_queue<BasicCube*, std::vector<BasicCube*>, Compare_cubes>();
    current_cubes.push(new BasicCube(step_start_cube));
    while(!current_cubes.empty()){
        BasicCube* cube = current_cubes.top();
        current_cubes.pop();
        for(Move* move : this->all_possible_moves){
            if(cube->solution_length > 0 && !check_move_sanity(cube->solution_path->back(), move)){
                continue;
            }
            BasicCube* new_cube = new BasicCube(cube);
            new_cube->rotate(move);

            size_t heuristics = (this->*check_step_completion)(new_cube);

            if(heuristics == 0){
                new_cube->temp_length = 0;
                *solved_cube = new BasicCube(new_cube);
                delete new_cube;
                delete cube;
                while(!current_cubes.empty()){
                    BasicCube* temp = current_cubes.top();
                    current_cubes.pop();
                    delete temp;
                }
                return 0;
            }

            new_cube->temp_length_plus_heuristic = new_cube->temp_length + ((double)heuristics) / 6;

            if(new_cube->temp_length < max_depth){
                current_cubes.push(new_cube);
            }
            else{
                delete new_cube;
            }
        }
        delete cube;
    }
    return -1;
}
