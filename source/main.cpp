#include "../inc/basic_cube.hpp"
#include <iostream>
#include "../inc/thistlethwaite.hpp"
#include <bitset>

static void inline print_move(Move* move);

int main(){
    auto cube = new BasicCube();
    // debug();

    // Move* move = new Move(Side::Up, 0, true, false);
    // cube->rotate(move);
    // // cube->print_cube();
    // // std::cout << move->to_string() << std::endl;
    // delete move;


    // move = new Move(Side::Right, 0, true, false);
    // cube->rotate(move);
    // // cube->print_cube();
    // delete move;


    // move = new Move(Side::Up, 0, true, false);
    // cube->rotate(move);
    // // cube->print_cube();
    // delete move;


    // move = new Move(Side::Down, 0, true, false);
    // cube->rotate(move);
    // // cube->print_cube();
    // delete move;

    // move = new Move(Side::Left, 0, false, false);
    // cube->rotate(move);
    // // cube->print_cube();
    // delete move;


    // move = new Move(Side::Back, 0, true, false);
    // cube->rotate(move);
    // // cube->print_cube();
    // // std::cout << move->to_string() << std::endl;
    // delete move;

    // move = new Move(Side::Front, 0, true, true);
    // cube->rotate(move);
    // // cube->print_cube();
    // delete move;


    // move = new Move(Side::Down, 0, false, false);
    // cube->rotate(move);
    // // cube->print_cube();
    // delete move;

    cube->rotate_sequence("U2 D2 B2 D' U' L2 R' F2 L2 R' U2 F L2 D2 R' B R' U2 D2 R2 B' U2 D R2 U");

    cube->print_cube();
    cube->clear_solution_path();

    auto thistlethwaite = new Thistlethwaite(cube);
    auto result = thistlethwaite->solve();
    if(result == 0){
        thistlethwaite->solved_cube->print_cube();
        thistlethwaite->solved_cube->print_solution_path();      
        std::cout << "is ok: " << thistlethwaite->solved_cube->check_solution();      
    }
    else{
        std::cout << "solution not found" << std::endl;
    }
    delete thistlethwaite;
    
    delete cube;
    return 0;
}

// 0L 0R 0F2 0U 0L 0U 0D 0F 0R' 0D2 0F 0B2 0L' 0F 0L 0U2 0F 0L 0F2 0L 0R 0D2 0L2 0U2 0L 0U2 0B2 0L2 0U2 0L

static void inline print_move(Move* move){
    std::cout << move->to_string() << std::endl;
}