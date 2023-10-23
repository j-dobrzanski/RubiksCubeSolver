#include "../inc/basic_cube.hpp"
#include <cstdint>
#include <iostream>
#include <algorithm>

static inline int check_solution_side(Side current_side_enum, std::vector<std::vector<BasicCubeTile*>*>* current_side, uint8_t cube_size);
static inline int rotate_side(std::vector<std::vector<BasicCubeTile*>*>** side_p, uint8_t size, bool clockwise);
static inline void delete_side(std::vector<std::vector<BasicCubeTile*>*>* side, uint8_t size);
static inline int compare_side(std::vector<std::vector<BasicCubeTile*>*>* side_a, std::vector<std::vector<BasicCubeTile*>*>* side_b, uint8_t size);
static inline std::vector<std::vector<BasicCubeTile*>*>* copy_side(std::vector<std::vector<BasicCubeTile*>*>* side, uint8_t size);


BasicCube::BasicCube(uint8_t size){
    BasicCube::size = size;
    BasicCube::side_front = initialize_side(Side::Front);
    BasicCube::side_back = initialize_side(Side::Back);
    BasicCube::side_left = initialize_side(Side::Left);
    BasicCube::side_right = initialize_side(Side::Right);
    BasicCube::side_up = initialize_side(Side::Up);
    BasicCube::side_down = initialize_side(Side::Down);
    BasicCube::solution_path = new std::vector<Move*>;
    this->solution_length = 0;
}

BasicCube::BasicCube(BasicCube* cube){
    this->size = cube->size;
    this->solution_path = new std::vector<Move*>;
    for(auto move : *cube->solution_path){
        this->solution_path->push_back(new Move(move));
    }
    this->side_front = copy_side(cube->side_front, cube->size);
    this->side_back = copy_side(cube->side_back, cube->size);
    this->side_left = copy_side(cube->side_left, cube->size);
    this->side_right = copy_side(cube->side_right, cube->size);
    this->side_up = copy_side(cube->side_up, cube->size);
    this->side_down = copy_side(cube->side_down, cube->size);
    this->solution_length = cube->solution_length;

}

std::vector<std::vector<BasicCubeTile*>*>* BasicCube::initialize_side(Side side){
    std::vector<std::vector<BasicCubeTile*>*>* new_side = new std::vector<std::vector<BasicCubeTile*>*>;
    for(size_t i = 0; i < size; i++){
        new_side->push_back(new std::vector<BasicCubeTile*>);
        for(size_t j = 0; j < size; j++){
            (*new_side)[i]->push_back(
                                new BasicCubeTile(side));
        }
    }
    return new_side;
}

int BasicCube::rotate_left(uint8_t tile_index, bool clockwise){
    if(tile_index == 0){
        rotate_side(&(BasicCube::side_left), BasicCube::size, clockwise);
    }
    else if(tile_index == BasicCube::size - 1){
        rotate_side(&(BasicCube::side_right), BasicCube::size, !clockwise);
    }
    // rotate other parts
    for(uint8_t i = 0; i < BasicCube::size; i++){
        auto up = (*(*BasicCube::side_up)[i])[tile_index];
        auto front= (*(*BasicCube::side_front)[i])[tile_index];
        auto down = (*(*BasicCube::side_down)[BasicCube::size - 1 - i])[tile_index];
        auto back = (*(*BasicCube::side_back)[BasicCube::size - 1 - i])[tile_index];

        if(clockwise){
            (*(*BasicCube::side_up)[i])[tile_index] = back;
            (*(*BasicCube::side_front)[i])[tile_index] = up;
            (*(*BasicCube::side_down)[BasicCube::size - 1 - i])[tile_index] = front;
            (*(*BasicCube::side_back)[BasicCube::size - 1 - i])[tile_index] = down;                
        }
        else{
            (*(*BasicCube::side_up)[i])[tile_index] = front;
            (*(*BasicCube::side_front)[i])[tile_index] = down;
            (*(*BasicCube::side_down)[BasicCube::size - 1 - i])[tile_index] = back;
            (*(*BasicCube::side_back)[BasicCube::size - 1 - i])[tile_index] = up;
        }

    }
    //
    return 0;
}

int BasicCube::rotate_right(uint8_t tile_index, bool clockwise){
    return BasicCube::rotate_left(BasicCube::size - 1 - tile_index, !clockwise);
}
int BasicCube::rotate_front(uint8_t tile_index, bool clockwise){
    if(tile_index == 0){
        rotate_side(&(BasicCube::side_front), BasicCube::size, clockwise);
    }
    else if(tile_index == BasicCube::size - 1){
        rotate_side(&(BasicCube::side_back), BasicCube::size, !clockwise);
    }
    // rotate other parts
    for(uint8_t i = 0; i < BasicCube::size; i++){
        auto up = (*(*BasicCube::side_up)[BasicCube::size - 1 - tile_index])[i];
        auto right = (*(*BasicCube::side_right)[i])[tile_index];
        auto down = (*(*BasicCube::side_down)[BasicCube::size - 1 - tile_index])[BasicCube::size - 1 - i];
        auto left = (*(*BasicCube::side_left)[BasicCube::size - 1 - i])[BasicCube::size - 1 - tile_index];

        if(clockwise){
            (*(*BasicCube::side_up)[BasicCube::size - 1 - tile_index])[i] = left;
            (*(*BasicCube::side_right)[i])[tile_index] = up;
            (*(*BasicCube::side_down)[BasicCube::size - 1 - tile_index])[BasicCube::size - 1 - i] = right;
            (*(*BasicCube::side_left)[BasicCube::size - 1 - i])[BasicCube::size - 1 - tile_index] = down;
        }
        else{
            (*(*BasicCube::side_up)[BasicCube::size - 1 - tile_index])[i] = right;
            (*(*BasicCube::side_right)[i])[tile_index] = down;
            (*(*BasicCube::side_down)[BasicCube::size - 1 - tile_index])[BasicCube::size - 1 - i] = left;
            (*(*BasicCube::side_left)[BasicCube::size - 1 - i])[BasicCube::size - 1 - tile_index] = up;
        }
    }
    //
    return 0;
}
int BasicCube::rotate_back(uint8_t tile_index, bool clockwise){
    return BasicCube::rotate_front(BasicCube::size - 1 - tile_index, !clockwise);
}
int BasicCube::rotate_up(uint8_t tile_index, bool clockwise){
    if(tile_index == 0){
        rotate_side(&(BasicCube::side_up), BasicCube::size, clockwise);
    }
    // else if(tile_index == BasicCube::size - 1){
    //     rotate_side(&(BasicCube::side_down), BasicCube::size, !clockwise);
    // }
    // rotate other parts
    auto front_row = (*BasicCube::side_front)[tile_index];
    auto right_row = (*BasicCube::side_right)[tile_index];
    auto back_row = (*BasicCube::side_back)[tile_index];
    auto left_row = (*BasicCube::side_left)[tile_index];
    std::reverse(back_row->begin(), back_row->end());
    if(clockwise){
        (*BasicCube::side_front)[tile_index] = right_row;
        (*BasicCube::side_right)[tile_index] = back_row;
        std::reverse(left_row->begin(), left_row->end());
        (*BasicCube::side_back)[tile_index] = left_row;
        (*BasicCube::side_left)[tile_index] = front_row;
    }
    else{
        (*BasicCube::side_front)[tile_index] = left_row;
        (*BasicCube::side_right)[tile_index] = front_row;
        std::reverse(right_row->begin(), right_row->end());
        (*BasicCube::side_back)[tile_index] = right_row;
        (*BasicCube::side_left)[tile_index] = back_row;
    }
    //
    return 0;
}
int BasicCube::rotate_down(uint8_t tile_index, bool clockwise){
    // return rotate_up(BasicCube::size - 1 - tile_index, !clockwise);
    if(tile_index == 0){
        rotate_side(&(BasicCube::side_down), BasicCube::size, !clockwise);
    }
    else if(tile_index == BasicCube::size - 1){
        rotate_side(&(BasicCube::side_up), BasicCube::size, !clockwise);
    }
    tile_index = BasicCube::size - 1 - tile_index;
    // rotate other parts
    auto front_row = (*BasicCube::side_front)[tile_index];
    auto right_row = (*BasicCube::side_right)[tile_index];
    auto back_row = (*BasicCube::side_back)[tile_index];
    auto left_row = (*BasicCube::side_left)[tile_index];
    std::reverse(back_row->begin(), back_row->end());
    if(clockwise){
        (*BasicCube::side_front)[tile_index] = left_row;
        (*BasicCube::side_right)[tile_index] = front_row;
        (*BasicCube::side_back)[tile_index] = right_row;
        (*BasicCube::side_left)[tile_index] = back_row;
    }
    else{
        (*BasicCube::side_front)[tile_index] = right_row;
        (*BasicCube::side_right)[tile_index] = back_row;
        (*BasicCube::side_back)[tile_index] = left_row;
        (*BasicCube::side_left)[tile_index] = front_row;
    }
    //
    return 0;
}

int BasicCube::check_solution(){
    int result = check_solution_side(Side::Front, BasicCube::side_front, BasicCube::size);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Back, BasicCube::side_back, BasicCube::size);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Left, BasicCube::side_left, BasicCube::size);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Right, BasicCube::side_right, BasicCube::size);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Up, BasicCube::side_up, BasicCube::size);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Down, BasicCube::side_down, BasicCube::size);
    if(result != 0){
        return result;
    }
    return 0;
}

void BasicCube::print_cube(){
    // printing up part
    for(uint8_t i = 0; i < BasicCube::size; i++){
        std::cout << std::string(3*BasicCube::size, ' ');
        std::cout << "  ";
        for(uint8_t j = 0; j < BasicCube::size; j++){
            std::cout << '[' << side_to_char((*(*BasicCube::side_up)[i])[j]->side_destination) << ']';
        }
        std::cout <<  std::endl;
    }
    std::cout << std::endl;
    // printing left/ front / right / back part

    for(uint8_t i = 0; i < BasicCube::size; i++){
        auto left = (*(*BasicCube::side_left)[i]);
        auto front = (*(*BasicCube::side_front)[i]);
        auto right = (*(*BasicCube::side_right)[i]);
        auto back = (*(*BasicCube::side_back)[i]);
        for(uint8_t j = 0; j < BasicCube::size; j++){
            std::cout << '[' << side_to_char(left[j]->side_destination) << ']';
        }
        std::cout << "  ";
        for(uint8_t j = 0; j < BasicCube::size; j++){
            std::cout << '[' << side_to_char(front[j]->side_destination) << ']';
        }
        std::cout << "  ";
        for(uint8_t j = 0; j < BasicCube::size; j++){
            std::cout << '[' << side_to_char(right[j]->side_destination) << ']';
        }
        std::cout << "  ";
        for(uint8_t j = 0; j < BasicCube::size; j++){
            std::cout << '[' << side_to_char(back[j]->side_destination) << ']';
        }
        std::cout << std::endl;;
    }
    std::cout << std::endl;

    // printing down part
    for(uint8_t i = 0; i < BasicCube::size; i++){
        std::cout << std::string(3*BasicCube::size, ' ');
        std::cout << "  ";
        for(uint8_t j = 0; j < BasicCube::size; j++){
            std::cout << '[' << side_to_char((*(*BasicCube::side_down)[BasicCube::size - 1- i])[j]->side_destination) << ']';
        }
        std::cout <<  std::endl;
    }
    std::cout << std::endl;
}

int BasicCube::compare_cube(BasicCube* cube){
    if(BasicCube::size != cube->size){
        return -1;
    }
    if(compare_side(BasicCube::side_front, cube->side_front, BasicCube::size)){
        return 1;
    }
    if(compare_side(BasicCube::side_back, cube->side_back, BasicCube::size)){
        return 2;
    }
    if(compare_side(BasicCube::side_left, cube->side_left, BasicCube::size)){
        return 3;
    }
    if(compare_side(BasicCube::side_right, cube->side_right, BasicCube::size)){
        return 4;
    }
    if(compare_side(BasicCube::side_up, cube->side_up, BasicCube::size)){
        return 5;
    }
    if(compare_side(BasicCube::side_down, cube->side_down, BasicCube::size)){
        return 6;
    }
    return 0;
}

int BasicCube::rotate(Move* move){
    this->solution_length++;
    BasicCube::solution_path->push_back(new Move(move));
    switch(move->side){
        case Side::Back:
            return BasicCube::rotate_back(move->tile_index, move->clockwise);
        case Side::Front:
            return BasicCube::rotate_front(move->tile_index, move->clockwise);
        case Side::Down:
            return BasicCube::rotate_down(move->tile_index, move->clockwise);
        case Side::Up:
            return BasicCube::rotate_up(move->tile_index, move->clockwise);
        case Side::Left:
            return BasicCube::rotate_left(move->tile_index, move->clockwise);
        case Side::Right:
            return BasicCube::rotate_right(move->tile_index, move->clockwise);
        default:
            return -1;
    }
}

int BasicCube::clear_solution_path(){
    for(auto move : *BasicCube::solution_path){
        delete move;
    }
    delete BasicCube::solution_path;
    BasicCube::solution_path = new std::vector<Move*>;
    this->solution_length = 0;
    return 0;
}

int BasicCube::print_solution_path(){
    for(auto move : *this->solution_path){
        std::cout << move->to_string() << " ";
    }
    std::cout << std::endl;
    return 0;
}

BasicCube::~BasicCube(){
    delete_side(BasicCube::side_front, BasicCube::size);
    delete_side(BasicCube::side_back, BasicCube::size);
    delete_side(BasicCube::side_left, BasicCube::size);
    delete_side(BasicCube::side_right, BasicCube::size);
    delete_side(BasicCube::side_up, BasicCube::size);
    delete_side(BasicCube::side_down, BasicCube::size);
    for(auto move : *BasicCube::solution_path){
        delete move;
    }
    delete solution_path;
}

static inline int check_solution_side(Side current_side_enum, std::vector<std::vector<BasicCubeTile*>*>* current_side, uint8_t cube_size){
    for(size_t i = 0; i < cube_size; i++){
        for(size_t j = 0; j < cube_size; j++){
            if(current_side_enum != (*(*current_side)[i])[j]->side_destination){
                return -1;
            }
        }
    }
    return 0;
}

static inline int rotate_side(std::vector<std::vector<BasicCubeTile*>*>** side_p, uint8_t size, bool clockwise){
    
    auto side = *side_p;
    std::vector<std::vector<BasicCubeTile*>*>* rotated = new std::vector<std::vector<BasicCubeTile*>*>();
    if(clockwise){
        for(int i = size - 1; i >= 0; i--){
            (*rotated).push_back(new std::vector<BasicCubeTile*>);
            for(int j = size - 1; j >= 0; j--){
                (*rotated)[size - 1 - i]->push_back((*(*side)[j])[size - 1 - i]);
            }
        }        
    }
    else{
        for(int i = size - 1; i >= 0; i--){
            (*rotated).push_back(new std::vector<BasicCubeTile*>);
            for(uint8_t j = 0; j < size; j++){
                (*rotated)[size - 1 - i]->push_back((*(*side)[j])[i]);
            }
        }        
    }

    *side_p = rotated;
    for(auto row : *side){
        delete row;
    }
    delete side;
    return 0;
}

static inline void delete_side(std::vector<std::vector<BasicCubeTile*>*>* side, uint8_t size){
    for(uint8_t i = 0; i < size; i++){
        for(uint8_t j = 0; j < size; j++){
            delete (*(*side)[i])[j];
        }
        delete (*side)[i];
    }
    delete side;
}

static inline int compare_side(std::vector<std::vector<BasicCubeTile*>*>* side_a, std::vector<std::vector<BasicCubeTile*>*>* side_b, uint8_t size){
    for(uint8_t i = 0; i < size; i++){
        for(uint8_t j = 0; j < size; j++){
            if((*(*side_a)[i])[j]->side_destination != (*(*side_b)[i])[j]->side_destination){
                return 1;
            }
        }
    }
    return 0;
}

static inline std::vector<std::vector<BasicCubeTile*>*>* copy_side(std::vector<std::vector<BasicCubeTile*>*>* side, uint8_t size){
    std::vector<std::vector<BasicCubeTile*>*>* new_side = new std::vector<std::vector<BasicCubeTile*>*>();
    for(uint8_t i = 0; i < size; i++){
        new_side->push_back(new std::vector<BasicCubeTile*>());
        for(uint8_t j = 0; j < size; j++){
            (*(*new_side)[i]).push_back(new BasicCubeTile((*(*side)[i])[j]->side_destination));
        }
    }
    return new_side;
}

