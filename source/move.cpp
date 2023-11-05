#include "../inc/move.hpp"
#include <cctype>
#include <ctime>
#include <string>
#include <iostream>

char side_to_char(Side side){
    switch(side){
        case Side::Up:
            return 'U';
        case Side::Front:
            return 'F';
        case Side::Right:
            return 'R';
        case Side::Back:
            return 'B';
        case Side::Left:
            return 'L';
        case Side::Down:
            return 'D';
        default:
            return -1;
    }
}

Side char_to_side(char c){
    switch(c){
        case 'U':
            return Side::Up;
        case 'F':
            return Side::Front;
        case 'R':
            return Side::Right;
        case 'D':
            return Side::Down;
        case 'B':
            return Side::Back;
        case 'L':
            return Side::Left;
        default:
            return Side::Error;
    }
}

std::string Move::to_string(){
    std::string str = "";
    str.append(std::to_string(Move::tile_index));
    str += side_to_char(Move::side);
    if(is_half_turn){
        str.append("2");
    }
    else if(!clockwise){
        str.append("'");
    }
    return str;
}

Move::Move(Side side, uint8_t tile_index, bool clockwise, bool is_half_turn){
    Move::side = side;
    Move::tile_index = tile_index;
    Move::is_half_turn = is_half_turn;
    if(is_half_turn){
        Move::clockwise = true;
    }    
    else{
        Move::clockwise = clockwise;
    }
}

Move::Move(std::string str){
    Move::tile_index = 0;
    bool temp_is_end_of_tile_index = false;
    Move::side = Side::Front;
    Move::clockwise = true;
    Move::is_half_turn = false;
    for(char c : str){
        if(std::isdigit(c)){
            if(!temp_is_end_of_tile_index){
                tile_index *= 10;
                tile_index += c - '0';
            }
            else{
                if(c - '0' == 2){
                    Move::is_half_turn = true;
                }
            }
        }
        else if(std::isalpha(c)){
            Move::side = char_to_side(c);
            temp_is_end_of_tile_index = true;
        }
        else{
            if(c == '\''){
                clockwise = false;
            }
        }
    }
}

Move::Move(Move* move){
    this->side = move->side;
    this->tile_index = move->tile_index;
    this->clockwise = move->clockwise;
    this->is_half_turn = move->is_half_turn;
}