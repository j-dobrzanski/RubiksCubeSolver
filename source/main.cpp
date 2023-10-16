#include "../inc/basic_cube.hpp"
#include <iostream>

int main(){
    auto cube = new BasicCube(3);
    cube->print_cube();


    delete cube;
    return 0;
}