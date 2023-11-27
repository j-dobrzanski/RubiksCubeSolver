#include "../inc/basic_cube.hpp"
#include <iostream>
#include "../inc/thistlethwaite.hpp"
#include "../inc/n_cube_solver/n_cube_solver.hpp"
#include <bitset>
#include <chrono>

// static void print_move(Move* move);

int main(){
    std::string move_sequence;

    std::cout << "Wybrana konfiguracja: " << std::endl;
    std::cout << "\t rozmiar kostki: " << CUBE_SIZE << std::endl;
#ifdef NO_CUBE_PRINT
    std::cout << "\t kostki nie będą wyświetlane" << std::endl;
#else
    std::cout << "\t kostki będą wyświetlane" << std::endl;
#endif
    // std::cout << "Podaj sekwencję ruchów mieszającą kostkę: " << std::endl;
    // std::cin >> move_sequence;
    auto cube = new BasicCube();
    // debug();

    cube->rotate_sequence("1U2 0D2 0B2 1D' 0U' 0L2 0R' 0F2 1L2 0R' 0U2 1F 1L2 0D2 1R' 0B 0R' 0U2 0D2 1R2 0B' 0U2 0D 0R2 0U");

    // cube->rotate_sequence("1U2 0D2 0B2 1D' 0U' 3L2 0R' 0F2 1L2 0R' 5U2 1F 1L2 3D2 1R' 7B 0R' 0U2 0D2 1R2 0B' 0U2 0D 0R2 0U 5U2 0D2 7B2 1D' 0U' 5L2 0R' 0F2 1L2 0R' 1U2 1F 1L2 0D2 1R' 2B 0R' 0U2 5D2 1R2 0B' 2U2 0D 3R2 0U  4U2 2D2 0B2 1D' 04U' 0L2 0R' 02F2 1L2 0R' 0U2 1F 1L2 3D2 1R' 0B 4R' 7U2 0D2 1R2 0B' 0U2 0D 8R2 0U ");


    cube->print_cube();
    cube->clear_solution_path();



    auto n_cube_solver = new NCubeSolver(cube);

    auto reducing_start = std::chrono::high_resolution_clock::now();
    int result = n_cube_solver->commutator_reduce();
    auto reducing_end = std::chrono::high_resolution_clock::now();

    if(result == 0){
        n_cube_solver->reduced_cube->print_cube();        
    }

    auto thistlethwaite = new Thistlethwaite(n_cube_solver->reduced_cube);
    auto thistlethwaite_solving_start = std::chrono::high_resolution_clock::now();
    result = thistlethwaite->solve();
    auto thistlethwaite_solving_end = std::chrono::high_resolution_clock::now();

    if(result == 0){
        thistlethwaite->solved_cube->print_cube();
        // thistlethwaite->solved_cube->print_solution_path();      
        std::cout << "Solution length: " << thistlethwaite->solved_cube->solution_length << std::endl;
        std::cout << "Reducing time: " << std::chrono::duration_cast<std::chrono::microseconds>(reducing_end - reducing_start).count() << "us" << std::endl;
        std::cout << "Solving time: " << std::chrono::duration_cast<std::chrono::microseconds>(thistlethwaite_solving_end - thistlethwaite_solving_start).count() << "us" << std::endl;
        // std::cout << "is ok: " << thistlethwaite->solved_cube->check_solution();      
    }
    else{
        std::cout << "solution not found" << std::endl;
    }
    delete thistlethwaite;    


    delete n_cube_solver;
    
    delete cube;
    return 0;
}

// 0L 0R 0F2 0U 0L 0U 0D 0F 0R' 0D2 0F 0B2 0L' 0F 0L 0U2 0F 0L 0F2 0L 0R 0D2 0L2 0U2 0L 0U2 0B2 0L2 0U2 0L

// static void print_move(Move* move){
//     std::cout << move->to_string() << std::endl;
// }