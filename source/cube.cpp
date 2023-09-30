#include "../inc/cube.hpp"
#include <cstdint>

Cube::Cube(uint8_t size){
    Cube::size = size;
    side_front = initialize_side(side::Front);
    side_back = initialize_side(side::Back);
    side_left = initialize_side(side::Left);
    side_right = initialize_side(side::Right);
    side_top = initialize_side(side::Top);
    side_bottom = initialize_side(side::Bottom);
}

std::vector<std::vector<CubeTile*>*>* Cube::initialize_side(side side){
    std::vector<std::vector<CubeTile*>*>* new_side = new std::vector<std::vector<CubeTile*>*>;
    for(size_t i = 0; i < size; i++){
        new_side->push_back(new std::vector<CubeTile*>);
        for(size_t j = 0; j < size; j++){
            // CubeTile* new_tile = new CubeTile(i, j, side, side, orientation::North, orientation::North);
            // auto check = new_side[i];
            // new_side[i].push_back(new_tile);
            new_side[i]->push_back(new CubeTile(i, j, side, side, orientation::North, orientation::North));
        }
    }
    return new_side;
}

int Cube::rotate(Axis axis, uint8_t tile_index){

}

int Cube::check_solution(){

}
