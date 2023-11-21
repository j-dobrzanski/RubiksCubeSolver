#include "../inc/basic_cube.hpp"
#include <iostream>
#include "../inc/thistlethwaite.hpp"
#include "../inc/n_cube_solver/n_cube_solver.hpp"
#include <bitset>

static void print_move(Move* move);

int main(){
    auto cube = new BasicCube();
    // debug();

    cube->rotate_sequence("1U2 0D2 0B2 1D' 0U' 0L2 0R' 0F2 1L2 0R' 0U2 1F 1L2 0D2 1R' 0B 0R' 0U2 0D2 1R2 0B' 0U2 0D 0R2 0U");

    cube->print_cube();
    cube->clear_solution_path();

    // auto thistlethwaite = new Thistlethwaite(cube);
    // auto result = thistlethwaite->solve();
    // if(result == 0){
    //     thistlethwaite->solved_cube->print_cube();
    //     thistlethwaite->solved_cube->print_solution_path();      
    //     std::cout << "is ok: " << thistlethwaite->solved_cube->check_solution();      
    // }
    // else{
    //     std::cout << "solution not found" << std::endl;
    // }
    // delete thistlethwaite;

    auto n_cube_solver = new NCubeSolver(cube);

    int result = n_cube_solver->reduce();

    if(result == 0){
        n_cube_solver->reduced_cube->print_cube();        
    }


    delete n_cube_solver;
    
    delete cube;
    return 0;
}

// 0L 0R 0F2 0U 0L 0U 0D 0F 0R' 0D2 0F 0B2 0L' 0F 0L 0U2 0F 0L 0F2 0L 0R 0D2 0L2 0U2 0L 0U2 0B2 0L2 0U2 0L

static void print_move(Move* move){
    std::cout << move->to_string() << std::endl;
}