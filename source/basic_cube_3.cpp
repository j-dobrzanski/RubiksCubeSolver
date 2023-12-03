#include "../inc/basic_cube_3.hpp"
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <utility>

static int check_solution_side(Side current_side_enum, std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* current_side);
static int rotate_side(std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>** side_p, bool clockwise);
static void delete_side(std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side);
static int compare_side(std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side_a, std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side_b);
static std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* copy_side(std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side);


BasicCube3::BasicCube3(){
    BasicCube3::side_front = initialize_side(Side::Front);
    BasicCube3::side_back = initialize_side(Side::Back);
    BasicCube3::side_left = initialize_side(Side::Left);
    BasicCube3::side_right = initialize_side(Side::Right);
    BasicCube3::side_up = initialize_side(Side::Up);
    BasicCube3::side_down = initialize_side(Side::Down);
    BasicCube3::solution_path = new std::vector<Move*>;
    this->solution_length = 0;
    this->temp_length = 0;

    this->sides = {
        {Side::Front, this->side_front},
        {Side::Back, this->side_back},
        {Side::Left, this->side_left},
        {Side::Right, this->side_right},
        {Side::Up, this->side_up},
        {Side::Down, this->side_down},
    };
}

BasicCube3::BasicCube3(BasicCube3* cube){
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

    this->sides = {
        {Side::Front, this->side_front},
        {Side::Back, this->side_back},
        {Side::Left, this->side_left},
        {Side::Right, this->side_right},
        {Side::Up, this->side_up},
        {Side::Down, this->side_down},
    };
}

BasicCube3::BasicCube3(BasicCube* cube){
    this->solution_path = new std::vector<Move*>();
    for(auto move : *cube->solution_path){
        this->solution_path->push_back(new Move(move));
    }
    this->solution_length = cube->solution_length;
    this->temp_length = cube->temp_length;
    this->temp_length_plus_heuristic = cube->temp_length_plus_heuristic;

    BasicCube3::side_front = initialize_side_from_cube(cube->side_front);
    BasicCube3::side_back = initialize_side_from_cube(cube->side_back);
    BasicCube3::side_left = initialize_side_from_cube(cube->side_left);
    BasicCube3::side_right = initialize_side_from_cube(cube->side_right);
    BasicCube3::side_up = initialize_side_from_cube(cube->side_up);
    BasicCube3::side_down = initialize_side_from_cube(cube->side_down);

    this->sides = {
        {Side::Front, this->side_front},
        {Side::Back, this->side_back},
        {Side::Left, this->side_left},
        {Side::Right, this->side_right},
        {Side::Up, this->side_up},
        {Side::Down, this->side_down},
    };
}

std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* BasicCube3::initialize_side(Side side){
    std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* new_side = new std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>();
    for(size_t i = 0; i < CUBE_SIZE_3; i++){
        for(size_t j = 0; j < CUBE_SIZE_3; j++){
            (*new_side)[i][j].side_destination = side;
        }
    }
    return new_side;
}

std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* BasicCube3::initialize_side_from_cube(std::array<std::array<BasicCubeTile, CUBE_SIZE>, CUBE_SIZE>* side){
    std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* new_side = new std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>();
    (*new_side)[0][0] = (*side)[0][0];
    (*new_side)[0][1] = (*side)[0][1];
    (*new_side)[0][2] = (*side)[0][CUBE_SIZE_LAST_INDEX];

    (*new_side)[1][0] = (*side)[1][0];
    (*new_side)[1][1] = (*side)[1][1];
    (*new_side)[1][2] = (*side)[1][CUBE_SIZE_LAST_INDEX];

    (*new_side)[2][0] = (*side)[CUBE_SIZE_LAST_INDEX][0];
    (*new_side)[2][1] = (*side)[CUBE_SIZE_LAST_INDEX][1];
    (*new_side)[2][2] = (*side)[CUBE_SIZE_LAST_INDEX][CUBE_SIZE_LAST_INDEX];
    return new_side;
}

int BasicCube3::rotate_left(size_t tile_index, bool clockwise){
    if(tile_index == 0){
        rotate_side(&(BasicCube3::side_left), clockwise);
        this->sides[Side::Left] = this->side_left;
    }
    else if(tile_index == CUBE_SIZE_3 - 1){
        rotate_side(&(BasicCube3::side_right), !clockwise);
        this->sides[Side::Right] = this->side_right;
    }
    // rotate other parts
    for(size_t i = 0; i < CUBE_SIZE_3; i++){
        auto up = (*BasicCube3::side_up)[i][tile_index];
        auto front= (*BasicCube3::side_front)[i][tile_index];
        auto down = (*BasicCube3::side_down)[CUBE_SIZE_3 - 1 - i][tile_index];
        auto back = (*BasicCube3::side_back)[CUBE_SIZE_3 - 1 - i][tile_index];

        if(clockwise){
            (*BasicCube3::side_up)[i][tile_index] = back;
            (*BasicCube3::side_front)[i][tile_index] = up;
            (*BasicCube3::side_down)[CUBE_SIZE_3 - 1 - i][tile_index] = front;
            (*BasicCube3::side_back)[CUBE_SIZE_3 - 1 - i][tile_index] = down;                
        }
        else{
            (*BasicCube3::side_up)[i][tile_index] = front;
            (*BasicCube3::side_front)[i][tile_index] = down;
            (*BasicCube3::side_down)[CUBE_SIZE_3 - 1 - i][tile_index] = back;
            (*BasicCube3::side_back)[CUBE_SIZE_3 - 1 - i][tile_index] = up;
        }

    }
    //
    return 0;
}

int BasicCube3::rotate_right(size_t tile_index, bool clockwise){
    return BasicCube3::rotate_left(CUBE_SIZE_3 - 1 - tile_index, !clockwise);
}
int BasicCube3::rotate_front(size_t tile_index, bool clockwise){
    if(tile_index == 0){
        rotate_side(&(BasicCube3::side_front), clockwise);
        this->sides[Side::Front] = this->side_front;
    }
    else if(tile_index == CUBE_SIZE_3 - 1){
        rotate_side(&(BasicCube3::side_back), clockwise);
        this->sides[Side::Back] = this->side_back;
    }
    // rotate other parts
    for(size_t i = 0; i < CUBE_SIZE_3; i++){
        auto up = (*BasicCube3::side_up)[CUBE_SIZE_3 - 1 - tile_index][i];
        auto right = (*BasicCube3::side_right)[i][tile_index];
        auto down = (*BasicCube3::side_down)[CUBE_SIZE_3 - 1 - tile_index][CUBE_SIZE_3 - 1 - i];
        auto left = (*BasicCube3::side_left)[CUBE_SIZE_3 - 1 - i][CUBE_SIZE_3 - 1 - tile_index];

        if(clockwise){
            (*BasicCube3::side_up)[CUBE_SIZE_3 - 1 - tile_index][i] = left;
            (*BasicCube3::side_right)[i][tile_index] = up;
            (*BasicCube3::side_down)[CUBE_SIZE_3 - 1 - tile_index][CUBE_SIZE_3 - 1 - i] = right;
            (*BasicCube3::side_left)[CUBE_SIZE_3 - 1 - i][CUBE_SIZE_3 - 1 - tile_index] = down;
        }
        else{
            (*BasicCube3::side_up)[CUBE_SIZE_3 - 1 - tile_index][i] = right;
            (*BasicCube3::side_right)[i][tile_index] = down;
            (*BasicCube3::side_down)[CUBE_SIZE_3 - 1 - tile_index][CUBE_SIZE_3 - 1 - i] = left;
            (*BasicCube3::side_left)[CUBE_SIZE_3 - 1 - i][CUBE_SIZE_3 - 1 - tile_index] = up;
        }
    }
    //
    return 0;
}
int BasicCube3::rotate_back(size_t tile_index, bool clockwise){
    return BasicCube3::rotate_front(CUBE_SIZE_3 - 1 - tile_index, !clockwise);
}
int BasicCube3::rotate_up(size_t tile_index, bool clockwise){
    if(tile_index == 0){
        rotate_side(&(BasicCube3::side_up), clockwise);
        this->sides[Side::Up] = this->side_up;        
    }
    else if(tile_index == CUBE_SIZE_3 - 1){
        rotate_side(&(BasicCube3::side_down), !clockwise);
    }
    // rotate other parts
    auto front_row = (*BasicCube3::side_front)[tile_index];
    auto right_row = (*BasicCube3::side_right)[tile_index];
    auto back_row = (*BasicCube3::side_back)[tile_index];
    auto left_row = (*BasicCube3::side_left)[tile_index];
    std::reverse(back_row.begin(), back_row.end());
    if(clockwise){
        (*BasicCube3::side_front)[tile_index] = right_row;
        (*BasicCube3::side_right)[tile_index] = back_row;
        std::reverse(left_row.begin(), left_row.end());
        (*BasicCube3::side_back)[tile_index] = left_row;
        (*BasicCube3::side_left)[tile_index] = front_row;
    }
    else{
        (*BasicCube3::side_front)[tile_index] = left_row;
        (*BasicCube3::side_right)[tile_index] = front_row;
        std::reverse(right_row.begin(), right_row.end());
        (*BasicCube3::side_back)[tile_index] = right_row;
        (*BasicCube3::side_left)[tile_index] = back_row;
    }
    //
    return 0;
}
int BasicCube3::rotate_down(size_t tile_index, bool clockwise){
    // return rotate_up(CUBE_SIZE_3 - 1 - tile_index, !clockwise);
    if(tile_index == 0){
        rotate_side(&(BasicCube3::side_down), !clockwise);
        this->sides[Side::Down] = this->side_down;
    }
    else if(tile_index == CUBE_SIZE_3 - 1){
        rotate_side(&(BasicCube3::side_up), !clockwise);
        this->sides[Side::Up] = this->side_up;
    }
    tile_index = CUBE_SIZE_3 - 1 - tile_index;
    // rotate other parts
    auto front_row = (*BasicCube3::side_front)[tile_index];
    auto right_row = (*BasicCube3::side_right)[tile_index];
    auto back_row = (*BasicCube3::side_back)[tile_index];
    auto left_row = (*BasicCube3::side_left)[tile_index];
    std::reverse(back_row.begin(), back_row.end());
    if(clockwise){
        (*BasicCube3::side_front)[tile_index] = left_row;
        (*BasicCube3::side_right)[tile_index] = front_row;
        std::reverse(right_row.begin(), right_row.end());
        (*BasicCube3::side_back)[tile_index] = right_row;
        (*BasicCube3::side_left)[tile_index] = back_row;
    }
    else{
        (*BasicCube3::side_front)[tile_index] = right_row;
        (*BasicCube3::side_right)[tile_index] = back_row;
        std::reverse(left_row.begin(), left_row.end());
        (*BasicCube3::side_back)[tile_index] = left_row;
        (*BasicCube3::side_left)[tile_index] = front_row;
    }
    //
    return 0;
}

int BasicCube3::check_solution(){
    int result = check_solution_side(Side::Front, BasicCube3::side_front);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Back, BasicCube3::side_back);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Left, BasicCube3::side_left);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Right, BasicCube3::side_right);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Up, BasicCube3::side_up);
    if(result != 0){
        return result;
    }
    result = check_solution_side(Side::Down, BasicCube3::side_down);
    if(result != 0){
        return result;
    }
    return 0;
}

void BasicCube3::print_cube(){
#ifndef NO_CUBE_PRINT
    // printing up part
    for(size_t i = 0; i < CUBE_SIZE_3; i++){
        std::cout << std::string(3*CUBE_SIZE_3, ' ');
        std::cout << "  ";
        for(size_t j = 0; j < CUBE_SIZE_3; j++){
            std::cout << '[' << side_to_char((*BasicCube3::side_up)[i][j].side_destination) << ']';
        }
        std::cout <<  std::endl;
    }
    std::cout << std::endl;
    // printing left/ front / right / back part

    for(size_t i = 0; i < CUBE_SIZE_3; i++){
        auto left = (*BasicCube3::side_left)[i];
        auto front = (*BasicCube3::side_front)[i];
        auto right = (*BasicCube3::side_right)[i];
        auto back = (*BasicCube3::side_back)[i];
        for(size_t j = 0; j < CUBE_SIZE_3; j++){
            std::cout << '[' << side_to_char(left[j].side_destination) << ']';
        }
        std::cout << "  ";
        for(size_t j = 0; j < CUBE_SIZE_3; j++){
            std::cout << '[' << side_to_char(front[j].side_destination) << ']';
        }
        std::cout << "  ";
        for(size_t j = 0; j < CUBE_SIZE_3; j++){
            std::cout << '[' << side_to_char(right[j].side_destination) << ']';
        }
        std::cout << "  ";
        for(size_t j = 0; j < CUBE_SIZE_3; j++){
            std::cout << '[' << side_to_char(back[j].side_destination) << ']';
        }
        std::cout << std::endl;;
    }
    std::cout << std::endl;

    // printing down part
    for(size_t i = 0; i < CUBE_SIZE_3; i++){
        std::cout << std::string(3*CUBE_SIZE_3, ' ');
        std::cout << "  ";
        for(size_t j = 0; j < CUBE_SIZE_3; j++){
            std::cout << '[' << side_to_char((*BasicCube3::side_down)[CUBE_SIZE_3 - 1- i][j].side_destination) << ']';
        }
        std::cout <<  std::endl;
    }
    std::cout << std::endl;
#endif    
}

int BasicCube3::compare_cube(BasicCube3* cube){
    if(compare_side(BasicCube3::side_front, cube->side_front)){
        return 1;
    }
    if(compare_side(BasicCube3::side_back, cube->side_back)){
        return 2;
    }
    if(compare_side(BasicCube3::side_left, cube->side_left)){
        return 3;
    }
    if(compare_side(BasicCube3::side_right, cube->side_right)){
        return 4;
    }
    if(compare_side(BasicCube3::side_up, cube->side_up)){
        return 5;
    }
    if(compare_side(BasicCube3::side_down, cube->side_down)){
        return 6;
    }
    return 0;
}

int BasicCube3::rotate(Move* move){
    this->solution_length++;
    this->temp_length++;
    BasicCube3::solution_path->push_back(new Move(move));
    if(move->is_half_turn){
        switch(move->side){
            case Side::Back:
                return std::min(BasicCube3::rotate_back(move->tile_index, move->clockwise),
                                BasicCube3::rotate_back(move->tile_index, move->clockwise));
            case Side::Front:
                return std::min(BasicCube3::rotate_front(move->tile_index, move->clockwise),
                                BasicCube3::rotate_front(move->tile_index, move->clockwise));
            case Side::Down:
                return std::min(BasicCube3::rotate_down(move->tile_index, move->clockwise),
                                BasicCube3::rotate_down(move->tile_index, move->clockwise));
            case Side::Up:
                return std::min(BasicCube3::rotate_up(move->tile_index, move->clockwise),
                                BasicCube3::rotate_up(move->tile_index, move->clockwise));
            case Side::Left:
                return std::min(BasicCube3::rotate_left(move->tile_index, move->clockwise),
                                BasicCube3::rotate_left(move->tile_index, move->clockwise));
            case Side::Right:
                return std::min(BasicCube3::rotate_right(move->tile_index, move->clockwise),
                                BasicCube3::rotate_right(move->tile_index, move->clockwise));
            default:
                return -1;
        }
    }
    else{
        switch(move->side){
            case Side::Back:
                return BasicCube3::rotate_back(move->tile_index, move->clockwise);
            case Side::Front:
                return BasicCube3::rotate_front(move->tile_index, move->clockwise);
            case Side::Down:
                return BasicCube3::rotate_down(move->tile_index, move->clockwise);
            case Side::Up:
                return BasicCube3::rotate_up(move->tile_index, move->clockwise);
            case Side::Left:
                return BasicCube3::rotate_left(move->tile_index, move->clockwise);
            case Side::Right:
                return BasicCube3::rotate_right(move->tile_index, move->clockwise);
            default:
                return -1;
        }        
    }
}

// int BasicCube3::clear_solution_path(){
//     for(auto move : *BasicCube3::solution_path){
//         delete move;
//     }
//     delete BasicCube3::solution_path;
//     BasicCube3::solution_path = new std::vector<Move*>;
//     this->solution_length = 0;
//     this->temp_length = 0;
//     return 0;
// }

// int BasicCube3::print_solution_path(){
//     if(this->solution_path != NULL){
//         for(auto move : *this->solution_path){
//             std::cout << move->to_string() << " ";
//         }        
//         std::cout << std::endl;
//     }
//     else{
//         std::cout << "Cube not moved" << std::endl;
//     }

//     return 0;
// }

std::tuple<Side, Side, Side> BasicCube3::get_corner(Side left_or_right, Side up_or_down, Side front_or_back){
    if(left_or_right == Side::Left &&
            up_or_down == Side::Up &&
            front_or_back == Side::Front){
        return std::make_tuple(
            (*this->side_left)[0][CUBE_SIZE_3 - 1].side_destination,
            (*this->side_up)[CUBE_SIZE_3 - 1][0].side_destination,
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
            (*this->side_left)[CUBE_SIZE_3 - 1][CUBE_SIZE_3 - 1].side_destination,
            (*this->side_down)[CUBE_SIZE_3 - 1][0].side_destination,
            (*this->side_front)[CUBE_SIZE_3 - 1][0].side_destination
        );
    }
    else if(left_or_right == Side::Left &&
            up_or_down == Side::Down &&
            front_or_back == Side::Back){
        return std::make_tuple(
            (*this->side_left)[CUBE_SIZE_3 - 1][0].side_destination,
            (*this->side_down)[0][0].side_destination,
            (*this->side_back)[CUBE_SIZE_3 - 1][0].side_destination
        );
    }
    else if(left_or_right == Side::Right &&
            up_or_down == Side::Up &&
            front_or_back == Side::Front){
        return std::make_tuple(
            (*this->side_right)[0][0].side_destination,
            (*this->side_up)[CUBE_SIZE_3 - 1][CUBE_SIZE_3 - 1].side_destination,
            (*this->side_front)[0][CUBE_SIZE_3 - 1].side_destination
        );
    }
    else if(left_or_right == Side::Right &&
            up_or_down == Side::Up &&
            front_or_back == Side::Back){
        return std::make_tuple(
            (*this->side_right)[0][CUBE_SIZE_3 - 1].side_destination,
            (*this->side_up)[0][CUBE_SIZE_3 - 1].side_destination,
            (*this->side_back)[0][CUBE_SIZE_3 - 1].side_destination
        );
    }
    else if(left_or_right == Side::Right &&
            up_or_down == Side::Down &&
            front_or_back == Side::Front){
        return std::make_tuple(
            (*this->side_right)[CUBE_SIZE_3 - 1][0].side_destination,
            (*this->side_down)[CUBE_SIZE_3 - 1][CUBE_SIZE_3 - 1].side_destination,
            (*this->side_front)[CUBE_SIZE_3 - 1][CUBE_SIZE_3 - 1].side_destination
        );
    }
    else if(left_or_right == Side::Right &&
            up_or_down == Side::Down &&
            front_or_back == Side::Back){
        return std::make_tuple(
            (*this->side_right)[CUBE_SIZE_3 - 1][CUBE_SIZE_3 - 1].side_destination,
            (*this->side_down)[0][CUBE_SIZE_3 - 1].side_destination,
            (*this->side_back)[CUBE_SIZE_3 - 1][CUBE_SIZE_3 - 1].side_destination
        );
    }
    return std::make_tuple(Side::Front, Side::Front, Side::Front);
}

// int BasicCube3::rotate_sequence(std::string move_sequence){
//     size_t length = move_sequence.size();
//     size_t current_index = 0;
//     while(current_index < length){
//         size_t next_blank = move_sequence.find(' ', current_index);
//         if(next_blank == move_sequence.npos){
//             next_blank = length;
//         }
//         Move* next_move = new Move(move_sequence.substr(current_index, next_blank-current_index));
//         this->rotate(next_move);
//         // std::cout << move_sequence.substr(current_index, next_blank-current_index) << std::endl;
//         // this->print_cube();
//         delete next_move;
//         current_index = next_blank + 1;
//     }
//     return 0;
// }

int BasicCube3::clear_solution_path(){
    for(auto move : *BasicCube3::solution_path){
        delete move;
    }
    delete BasicCube3::solution_path;
    BasicCube3::solution_path = new std::vector<Move*>;
    this->solution_length = 0;
    this->temp_length = 0;
    return 0;
}

int BasicCube3::print_solution_path(){
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

BasicCube3::~BasicCube3(){
    delete_side(BasicCube3::side_front);
    delete_side(BasicCube3::side_back);
    delete_side(BasicCube3::side_left);
    delete_side(BasicCube3::side_right);
    delete_side(BasicCube3::side_up);
    delete_side(BasicCube3::side_down);
    for(auto move : *BasicCube3::solution_path){
        delete move;
    }
    delete solution_path;
}

static int check_solution_side(Side current_side_enum, std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* current_side){
    for(size_t i = 0; i < CUBE_SIZE_3; i++){
        for(size_t j = 0; j < CUBE_SIZE_3; j++){
            if(current_side_enum != (*current_side)[i][j].side_destination){
                return 1;
            }
        }
    }
    return 0;
}

static int rotate_side(std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>** side_p, bool clockwise){
    
    auto side = *side_p;
    std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* rotated = new std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>();
    if(clockwise){
        for(int i = CUBE_SIZE_3 - 1; i >= 0; i--){
            for(int j = CUBE_SIZE_3 - 1; j >= 0; j--){
                (*rotated)[CUBE_SIZE_3 - 1 - i][CUBE_SIZE_3 - 1 - j].side_destination = (*side)[j][CUBE_SIZE_3 - 1 - i].side_destination;
            }
        }        
    }
    else{
        for(int i = CUBE_SIZE_3 - 1; i >= 0; i--){
            for(size_t j = 0; j < CUBE_SIZE_3; j++){
                (*rotated)[CUBE_SIZE_3 - 1 - i][j].side_destination = (*side)[j][i].side_destination;
            }
        }        
    }

    *side_p = rotated;

    delete side;
    return 0;
}

static void delete_side(std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side){
    delete side;
}

static int compare_side(std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side_a, std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side_b){
    for(size_t i = 0; i < CUBE_SIZE_3; i++){
        for(size_t j = 0; j < CUBE_SIZE_3; j++){
            if((*side_a)[i][j].side_destination != (*side_b)[i][j].side_destination){
                return 1;
            }
        }
    }
    return 0;
}

static std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* copy_side(std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* side){
    std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>* new_side = new std::array<std::array<BasicCubeTile, CUBE_SIZE_3>, CUBE_SIZE_3>();
    for(size_t i = 0; i < CUBE_SIZE_3; i++){
        for(size_t j = 0; j < CUBE_SIZE_3; j++){
            (*new_side)[i][j].side_destination = (*side)[i][j].side_destination;
        }
    }
    return new_side;
}

