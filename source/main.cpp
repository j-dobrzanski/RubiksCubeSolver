#include "../inc/basic_cube.hpp"
#include <iostream>
#include "../inc/thistlethwaite.hpp"

static void inline print_move(Move* move);

int main(){
    auto cube = new BasicCube();
    std::cout << sizeof(*cube) << std::endl; 
    cube->print_cube();

    // std::cout << thistlethwaite->phase3_check(cube) << std::endl;

    Move* move = new Move(Side::Up, 0, true, false);
    cube->rotate(move);
    // print_move(move);
    delete move;

    // cube->print_cube();

    move = new Move(Side::Right, 0, true, false);
    cube->rotate(move);
    // print_move(move);
    delete move;

    // cube->print_cube();

    move = new Move(Side::Up, 0, true, false);
    cube->rotate(move);
    // print_move(move);
    delete move;

    // cube->print_cube();

    move = new Move(Side::Down, 0, true, false);
    cube->rotate(move);
    delete move;


    cube->print_cube();
    cube->clear_solution_path();
    // std::cout << thistlethwaite->phase3_check(cube) << std::endl;
    auto thistlethwaite = new Thistlethwaite(cube);
    std::cout << thistlethwaite->phase1_check(cube) << std::endl;
    thistlethwaite->solve();
    thistlethwaite->solved_cube->print_cube();
    thistlethwaite->solved_cube->print_solution_path();

    delete thistlethwaite;
    delete cube;
    return 0;
}

static void inline print_move(Move* move){
    std::cout << move->to_string() << std::endl;
}