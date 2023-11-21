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


bool check_move_sanity(Move* last_move, Move* new_move){
    // Let's see that there is no need to move the same side twice, as:
    //  - two same quarter moves == half move - already calculated the step before
    //  - two same half moves == empty move - no need to go back to the step before
    //  - clockwise + counterclockwise move == empty move - 
    //                  no need to go back to the step before
    //  - quarter move + half move == quarter move of opposing clockwise
    if(last_move->side == new_move->side &&
        last_move->tile_index == new_move->tile_index){
        return false;
    }


    // Also when two moves don't interfere with each other 
    // then their order doesn't matter.
    // This means we can restrict moves only to come up in specific order:
    //  - only F -> B, no Front move can occure directly after Back move
    //  - only L -> R, no Left move can occure directly after Right move
    //  - only U -> D, no Up move can occure directly after Down move
    if((last_move->side == Side::Back && new_move->side == Side::Front) ||
            (last_move->side == Side::Right && new_move->side == Side::Left) ||
            (last_move->side == Side::Down && new_move->side == Side::Up)){
        return false;
    }
    return true;
}