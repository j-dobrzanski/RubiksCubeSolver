#include "../inc/basic_cube.hpp"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <utility>

static inline int check_solution_side(Side current_side_enum, std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* current_side);
static inline int rotate_side(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>** side_p, bool clockwise);
static inline void delete_side(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side);
static inline int compare_side(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side_a, std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side_b);
static inline std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* copy_side(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side);


BasicCube::BasicCube(){
    BasicCube::side_front = initialize_side(Side::Front);
    BasicCube::side_back = initialize_side(Side::Back);
    BasicCube::side_left = initialize_side(Side::Left);
    BasicCube::side_right = initialize_side(Side::Right);
    BasicCube::side_up = initialize_side(Side::Up);
    BasicCube::side_down = initialize_side(Side::Down);
    BasicCube::solution_path = new std::vector<Move*>;
    this->solution_length = 0;
    this->temp_length = 0;
}

BasicCube::BasicCube(BasicCube* cube){
    this->solution_path = new std::vector<Move*>;
    for(auto move : *cube->solution_path){
        this->solution_path->push_back(new Move(move));
    }
    this->side_front = copy_side(cube->side_front);
    this->side_back = copy_side(cube->side_back);
    this->side_left = copy_side(cube->side_left);
    this->side_right = copy_side(cube->side_right);
    this->side_up = copy_side(cube->side_up);
    this->side_down = copy_side(cube->side_down);
    this->solution_length = cube->solution_length;
    this->temp_length = cube->temp_length;
}

std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* BasicCube::initialize_side(Side side){
    std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* new_side = new std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>();
    for(size_t i = 0; i < CUBE_SIZE; i++){
        for(size_t j = 0; j < CUBE_SIZE; j++){
            (*new_side)[i][j].side_destination = side;
        }
    }
    return new_side;
}

int BasicCube::rotate_left(uint8_t tile_index, bool clockwise){
    if(tile_index == 0){
        rotate_side(&(BasicCube::side_left), clockwise);
    }
    else if(tile_index == CUBE_SIZE - 1){
        rotate_side(&(BasicCube::side_right), !clockwise);
    }
    // rotate other parts
    for(uint8_t i = 0; i < CUBE_SIZE; i++){
        auto up = (*BasicCube::side_up)[i][tile_index];
        auto front= (*BasicCube::side_front)[i][tile_index];
        auto down = (*BasicCube::side_down)[CUBE_SIZE - 1 - i][tile_index];
        auto back = (*BasicCube::side_back)[CUBE_SIZE - 1 - i][tile_index];

        if(clockwise){
            (*BasicCube::side_up)[i][tile_index] = back;
            (*BasicCube::side_front)[i][tile_index] = up;
            (*BasicCube::side_down)[CUBE_SIZE - 1 - i][tile_index] = front;
            (*BasicCube::side_back)[CUBE_SIZE - 1 - i][tile_index] = down;                
        }
        else{
            (*BasicCube::side_up)[i][tile_index] = front;
            (*BasicCube::side_front)[i][tile_index] = down;
            (*BasicCube::side_down)[CUBE_SIZE - 1 - i][tile_index] = back;
            (*BasicCube::side_back)[CUBE_SIZE - 1 - i][tile_index] = up;
        }

    }
    //
    return 0;
}

int BasicCube::rotate_right(uint8_t tile_index, bool clockwise){
    return BasicCube::rotate_left(CUBE_SIZE - 1 - tile_index, !clockwise);
}
int BasicCube::rotate_front(uint8_t tile_index, bool clockwise){
    if(tile_index == 0){
        rotate_side(&(BasicCube::side_front), clockwise);
    }
    else if(tile_index == CUBE_SIZE - 1){
        rotate_side(&(BasicCube::side_back), clockwise);
    }
    // rotate other parts
    for(uint8_t i = 0; i < CUBE_SIZE; i++){
        auto up = (*BasicCube::side_up)[CUBE_SIZE - 1 - tile_index][i];
        auto right = (*BasicCube::side_right)[i][tile_index];
        auto down = (*BasicCube::side_down)[CUBE_SIZE - 1 - tile_index][CUBE_SIZE - 1 - i];
        auto left = (*BasicCube::side_left)[CUBE_SIZE - 1 - i][CUBE_SIZE - 1 - tile_index];

        if(clockwise){
            (*BasicCube::side_up)[CUBE_SIZE - 1 - tile_index][i] = left;
            (*BasicCube::side_right)[i][tile_index] = up;
            (*BasicCube::side_down)[CUBE_SIZE - 1 - tile_index][CUBE_SIZE - 1 - i] = right;
            (*BasicCube::side_left)[CUBE_SIZE - 1 - i][CUBE_SIZE - 1 - tile_index] = down;
        }
        else{
            (*BasicCube::side_up)[CUBE_SIZE - 1 - tile_index][i] = right;
            (*BasicCube::side_right)[i][tile_index] = down;
            (*BasicCube::side_down)[CUBE_SIZE - 1 - tile_index][CUBE_SIZE - 1 - i] = left;
            (*BasicCube::side_left)[CUBE_SIZE - 1 - i][CUBE_SIZE - 1 - tile_index] = up;
        }
    }
    //
    return 0;
}
int BasicCube::rotate_back(uint8_t tile_index, bool clockwise){
    return BasicCube::rotate_front(CUBE_SIZE - 1 - tile_index, !clockwise);
}
int BasicCube::rotate_up(uint8_t tile_index, bool clockwise){
    if(tile_index == 0){
        rotate_side(&(BasicCube::side_up), clockwise);
    }
    // else if(tile_index == CUBE_SIZE - 1){
    //     rotate_side(&(BasicCube::side_down), CUBE_SIZE, !clockwise);
    // }
    // rotate other parts
    auto front_row = (*BasicCube::side_front)[tile_index];
    auto right_row = (*BasicCube::side_right)[tile_index];
    auto back_row = (*BasicCube::side_back)[tile_index];
    auto left_row = (*BasicCube::side_left)[tile_index];
    std::reverse(back_row.begin(), back_row.end());
    if(clockwise){
        (*BasicCube::side_front)[tile_index] = right_row;
        (*BasicCube::side_right)[tile_index] = back_row;
        std::reverse(left_row.begin(), left_row.end());
        (*BasicCube::side_back)[tile_index] = left_row;
        (*BasicCube::side_left)[tile_index] = front_row;
    }
    else{
        (*BasicCube::side_front)[tile_index] = left_row;
        (*BasicCube::side_right)[tile_index] = front_row;
        std::reverse(right_row.begin(), right_row.end());
        (*BasicCube::side_back)[tile_index] = right_row;
        (*BasicCube::side_left)[tile_index] = back_row;
    }
    //
    return 0;
}
int BasicCube::rotate_down(uint8_t tile_index, bool clockwise){
    // return rotate_up(CUBE_SIZE - 1 - tile_index, !clockwise);
    if(tile_index == 0){
        rotate_side(&(BasicCube::side_down), !clockwise);
    }
    else if(tile_index == CUBE_SIZE - 1){
        rotate_side(&(BasicCube::side_up), !clockwise);
    }
    tile_index = CUBE_SIZE - 1 - tile_index;
    // rotate other parts
    auto front_row = (*BasicCube::side_front)[tile_index];
    auto right_row = (*BasicCube::side_right)[tile_index];
    auto back_row = (*BasicCube::side_back)[tile_index];
    auto left_row = (*BasicCube::side_left)[tile_index];
    std::reverse(back_row.begin(), back_row.end());
    if(clockwise){
        (*BasicCube::side_front)[tile_index] = left_row;
        (*BasicCube::side_right)[tile_index] = front_row;
        std::reverse(right_row.begin(), right_row.end());
        (*BasicCube::side_back)[tile_index] = right_row;
        (*BasicCube::side_left)[tile_index] = back_row;
    }
    else{
        (*BasicCube::side_front)[tile_index] = right_row;
        (*BasicCube::side_right)[tile_index] = back_row;
        std::reverse(left_row.begin(), left_row.end());
        (*BasicCube::side_back)[tile_index] = left_row;
        (*BasicCube::side_left)[tile_index] = front_row;
    }
    //
    return 0;
}

int BasicCube::check_solution(){
    int result = check_solution_side(Side::Front, BasicCube::side_front);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Back, BasicCube::side_back);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Left, BasicCube::side_left);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Right, BasicCube::side_right);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Up, BasicCube::side_up);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Down, BasicCube::side_down);
    if(result != 0){
        return result;
    }
    return 0;
}

void BasicCube::print_cube(){
    // printing up part
    for(uint8_t i = 0; i < CUBE_SIZE; i++){
        std::cout << std::string(3*CUBE_SIZE, ' ');
        std::cout << "  ";
        for(uint8_t j = 0; j < CUBE_SIZE; j++){
            std::cout << '[' << side_to_char((*BasicCube::side_up)[i][j].side_destination) << ']';
        }
        std::cout <<  std::endl;
    }
    std::cout << std::endl;
    // printing left/ front / right / back part

    for(uint8_t i = 0; i < CUBE_SIZE; i++){
        auto left = (*BasicCube::side_left)[i];
        auto front = (*BasicCube::side_front)[i];
        auto right = (*BasicCube::side_right)[i];
        auto back = (*BasicCube::side_back)[i];
        for(uint8_t j = 0; j < CUBE_SIZE; j++){
            std::cout << '[' << side_to_char(left[j].side_destination) << ']';
        }
        std::cout << "  ";
        for(uint8_t j = 0; j < CUBE_SIZE; j++){
            std::cout << '[' << side_to_char(front[j].side_destination) << ']';
        }
        std::cout << "  ";
        for(uint8_t j = 0; j < CUBE_SIZE; j++){
            std::cout << '[' << side_to_char(right[j].side_destination) << ']';
        }
        std::cout << "  ";
        for(uint8_t j = 0; j < CUBE_SIZE; j++){
            std::cout << '[' << side_to_char(back[j].side_destination) << ']';
        }
        std::cout << std::endl;;
    }
    std::cout << std::endl;

    // printing down part
    for(uint8_t i = 0; i < CUBE_SIZE; i++){
        std::cout << std::string(3*CUBE_SIZE, ' ');
        std::cout << "  ";
        for(uint8_t j = 0; j < CUBE_SIZE; j++){
            std::cout << '[' << side_to_char((*BasicCube::side_down)[CUBE_SIZE - 1- i][j].side_destination) << ']';
        }
        std::cout <<  std::endl;
    }
    std::cout << std::endl;
}

int BasicCube::compare_cube(BasicCube* cube){
    if(compare_side(BasicCube::side_front, cube->side_front)){
        return 1;
    }
    if(compare_side(BasicCube::side_back, cube->side_back)){
        return 2;
    }
    if(compare_side(BasicCube::side_left, cube->side_left)){
        return 3;
    }
    if(compare_side(BasicCube::side_right, cube->side_right)){
        return 4;
    }
    if(compare_side(BasicCube::side_up, cube->side_up)){
        return 5;
    }
    if(compare_side(BasicCube::side_down, cube->side_down)){
        return 6;
    }
    return 0;
}

int BasicCube::rotate(Move* move){
    this->solution_length++;
    this->temp_length++;
    BasicCube::solution_path->push_back(new Move(move));
    if(move->is_half_turn){
        switch(move->side){
            case Side::Back:
                return std::min(BasicCube::rotate_back(move->tile_index, move->clockwise),
                                BasicCube::rotate_back(move->tile_index, move->clockwise));
            case Side::Front:
                return std::min(BasicCube::rotate_front(move->tile_index, move->clockwise),
                                BasicCube::rotate_front(move->tile_index, move->clockwise));
            case Side::Down:
                return std::min(BasicCube::rotate_down(move->tile_index, move->clockwise),
                                BasicCube::rotate_down(move->tile_index, move->clockwise));
            case Side::Up:
                return std::min(BasicCube::rotate_up(move->tile_index, move->clockwise),
                                BasicCube::rotate_up(move->tile_index, move->clockwise));
            case Side::Left:
                return std::min(BasicCube::rotate_left(move->tile_index, move->clockwise),
                                BasicCube::rotate_left(move->tile_index, move->clockwise));
            case Side::Right:
                return std::min(BasicCube::rotate_right(move->tile_index, move->clockwise),
                                BasicCube::rotate_right(move->tile_index, move->clockwise));
            default:
                return -1;
        }
    }
    else{
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
}

int BasicCube::clear_solution_path(){
    for(auto move : *BasicCube::solution_path){
        delete move;
    }
    delete BasicCube::solution_path;
    BasicCube::solution_path = new std::vector<Move*>;
    this->solution_length = 0;
    this->temp_length = 0;
    return 0;
}

int BasicCube::print_solution_path(){
    if(this->solution_path != NULL){
        for(auto move : *this->solution_path){
            std::cout << move->to_string() << " ";
        }        
        std::cout << std::endl;
    }
    else{
        std::cout << "Cube not moved" << std::endl;
    }

    return 0;
}

std::tuple<Side, Side, Side> BasicCube::get_corner(Side left_or_right, Side up_or_down, Side front_or_back){
    if(left_or_right == Side::Left &&
            up_or_down == Side::Up &&
            front_or_back == Side::Front){
        return std::make_tuple(
            (*this->side_left)[0][CUBE_SIZE - 1].side_destination,
            (*this->side_up)[CUBE_SIZE - 1][0].side_destination,
            (*this->side_front)[0][0].side_destination
        );
    }
    else if(left_or_right == Side::Left &&
            up_or_down == Side::Up &&
            front_or_back == Side::Back){
        return std::make_tuple(
            (*this->side_left)[0][0].side_destination,
            (*this->side_up)[0][0].side_destination,
            (*this->side_back)[0][0].side_destination);
    }
    else if(left_or_right == Side::Left &&
            up_or_down == Side::Down &&
            front_or_back == Side::Front){
        return std::make_tuple(
            (*this->side_left)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination,
            (*this->side_down)[CUBE_SIZE - 1][0].side_destination,
            (*this->side_front)[CUBE_SIZE - 1][0].side_destination
        );
    }
    else if(left_or_right == Side::Left &&
            up_or_down == Side::Down &&
            front_or_back == Side::Back){
        return std::make_tuple(
            (*this->side_left)[CUBE_SIZE - 1][0].side_destination,
            (*this->side_down)[0][0].side_destination,
            (*this->side_back)[CUBE_SIZE - 1][0].side_destination
        );
    }
    else if(left_or_right == Side::Right &&
            up_or_down == Side::Up &&
            front_or_back == Side::Front){
        return std::make_tuple(
            (*this->side_right)[0][0].side_destination,
            (*this->side_up)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination,
            (*this->side_front)[0][CUBE_SIZE - 1].side_destination
        );
    }
    else if(left_or_right == Side::Right &&
            up_or_down == Side::Up &&
            front_or_back == Side::Back){
        return std::make_tuple(
            (*this->side_right)[0][CUBE_SIZE - 1].side_destination,
            (*this->side_up)[0][CUBE_SIZE - 1].side_destination,
            (*this->side_back)[0][CUBE_SIZE - 1].side_destination
        );
    }
    else if(left_or_right == Side::Right &&
            up_or_down == Side::Down &&
            front_or_back == Side::Front){
        return std::make_tuple(
            (*this->side_right)[CUBE_SIZE - 1][0].side_destination,
            (*this->side_down)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination,
            (*this->side_front)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination
        );
    }
    else if(left_or_right == Side::Right &&
            up_or_down == Side::Down &&
            front_or_back == Side::Back){
        return std::make_tuple(
            (*this->side_right)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination,
            (*this->side_down)[0][CUBE_SIZE - 1].side_destination,
            (*this->side_back)[CUBE_SIZE - 1][CUBE_SIZE - 1].side_destination
        );
    }
    return std::make_tuple(Side::Front, Side::Front, Side::Front);
}

std::pair<Side, Side> BasicCube::get_edge(Side side1, Side side2){
    if(side2 == Side::Left || side2 == Side::Right){
        auto temp = side1;
        side1 = side2;
        side2 = temp;
    }
    
    if(side1 == Side::Left){
        if(side2 == Side::Front){
            return std::make_pair((*this->side_left)[1][CUBE_SIZE - 1].side_destination, (*this->side_front)[1][0].side_destination);
        }
        else if(side2 == Side::Back){
            return std::make_pair((*this->side_left)[1][0].side_destination, (*this->side_back)[1][0].side_destination);
        }
        else if(side2 == Side::Up){
            return std::make_pair((*this->side_left)[0][1].side_destination, (*this->side_up)[1][0].side_destination);
        }
        else if(side2 == Side::Down){
            return std::make_pair((*this->side_left)[CUBE_SIZE - 1][1].side_destination, (*this->side_down)[1][0].side_destination);
        }
    }
    else if(side1 == Side::Right){
        if(side2 == Side::Front){
            return std::make_pair((*this->side_right)[1][0].side_destination, (*this->side_front)[1][CUBE_SIZE - 1].side_destination);
        }
        else if(side2 == Side::Back){
            return std::make_pair((*this->side_right)[1][CUBE_SIZE - 1].side_destination, (*this->side_back)[1][CUBE_SIZE - 1].side_destination);
        }
        else if(side2 == Side::Up){
            return std::make_pair((*this->side_right)[0][1].side_destination, (*this->side_up)[1][CUBE_SIZE - 1].side_destination);
        }
        else if(side2 == Side::Down){
            return std::make_pair((*this->side_right)[CUBE_SIZE - 1][1].side_destination, (*this->side_down)[1][CUBE_SIZE - 1].side_destination);
        }
    }
    else if((side1 == Side::Up && side2 == Side::Front) ||
            (side1 == Side::Front && side2 == Side::Up)){
        return std::make_pair((*this->side_front)[0][1].side_destination, (*this->side_up)[CUBE_SIZE - 1][1].side_destination);
    }
    else if((side1 == Side::Down && side2 == Side::Front) ||
            (side1 == Side::Front && side2 == Side::Down)){
        return std::make_pair((*this->side_front)[CUBE_SIZE - 1][1].side_destination, (*this->side_down)[CUBE_SIZE - 1][1].side_destination);
    }
    else if((side1 == Side::Up && side2 == Side::Back) ||
            (side1 == Side::Back && side2 == Side::Up)){
        return std::make_pair((*this->side_back)[0][1].side_destination, (*this->side_up)[0][1].side_destination);
    }
    else if((side1 == Side::Down && side2 == Side::Back) ||
            (side1 == Side::Back && side2 == Side::Down)){
        return std::make_pair((*this->side_back)[CUBE_SIZE - 1][1].side_destination, (*this->side_down)[0][1].side_destination);
    }
    return std::make_pair(Side::Front, Side::Front);
}

int BasicCube::rotate_sequence(std::string move_sequence){
    size_t length = move_sequence.size();
    size_t current_index = 0;
    while(current_index < length){
        size_t next_blank = move_sequence.find(' ', current_index);
        if(next_blank == move_sequence.npos){
            next_blank = length;
        }
        Move* next_move = new Move(move_sequence.substr(current_index, next_blank-current_index));
        this->rotate(next_move);
        delete next_move;
        current_index = next_blank + 1;
    }
    return 0;
}

BasicCube::~BasicCube(){
    delete_side(BasicCube::side_front);
    delete_side(BasicCube::side_back);
    delete_side(BasicCube::side_left);
    delete_side(BasicCube::side_right);
    delete_side(BasicCube::side_up);
    delete_side(BasicCube::side_down);
    for(auto move : *BasicCube::solution_path){
        delete move;
    }
    delete solution_path;
}

static inline int check_solution_side(Side current_side_enum, std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* current_side){
    for(size_t i = 0; i < CUBE_SIZE; i++){
        for(size_t j = 0; j < CUBE_SIZE; j++){
            if(current_side_enum != (*current_side)[i][j].side_destination){
                return 1;
            }
        }
    }
    return 0;
}

static inline int rotate_side(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>** side_p, bool clockwise){
    
    auto side = *side_p;
    std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* rotated = new std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>();
    if(clockwise){
        for(int i = CUBE_SIZE - 1; i >= 0; i--){
            for(int j = CUBE_SIZE - 1; j >= 0; j--){
                (*rotated)[CUBE_SIZE - 1 - i][CUBE_SIZE - 1 - j].side_destination = (*side)[j][CUBE_SIZE - 1 - i].side_destination;
            }
        }        
    }
    else{
        for(int i = CUBE_SIZE - 1; i >= 0; i--){
            for(uint8_t j = 0; j < CUBE_SIZE; j++){
                (*rotated)[CUBE_SIZE - 1 - i][j].side_destination = (*side)[j][i].side_destination;
            }
        }        
    }

    *side_p = rotated;

    delete side;
    return 0;
}

static inline void delete_side(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side){
    delete side;
}

static inline int compare_side(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side_a, std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side_b){
    for(uint8_t i = 0; i < CUBE_SIZE; i++){
        for(uint8_t j = 0; j < CUBE_SIZE; j++){
            if((*side_a)[i][j].side_destination != (*side_b)[i][j].side_destination){
                return 1;
            }
        }
    }
    return 0;
}

static inline std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* copy_side(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side){
    std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* new_side = new std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>();
    for(uint8_t i = 0; i < CUBE_SIZE; i++){
        for(uint8_t j = 0; j < CUBE_SIZE; j++){
            (*new_side)[i][j].side_destination = (*side)[i][j].side_destination;
        }
    }
    return new_side;
}

