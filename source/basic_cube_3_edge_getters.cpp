#include "../inc/basic_cube_3.hpp"
#include <array>
#include <utility>

static std::array<std::pair<Side, Side>, EDGE_LENGTH_3> reverse_edge(std::array<std::pair<Side, Side>, EDGE_LENGTH_3> edge);

std::pair<Side, Side> BasicCube3::get_solved_edge(Side side1, Side side2){
    if(side2 == Side::Left || side2 == Side::Right){
        auto temp = side1;
        side1 = side2;
        side2 = temp;
    }
    
    if(side1 == Side::Left){
        if(side2 == Side::Front){
            return std::make_pair((*this->side_left)[1][CUBE_SIZE_3 - 1].side_destination, (*this->side_front)[1][0].side_destination);
        }
        else if(side2 == Side::Back){
            return std::make_pair((*this->side_left)[1][0].side_destination, (*this->side_back)[1][0].side_destination);
        }
        else if(side2 == Side::Up){
            return std::make_pair((*this->side_left)[0][1].side_destination, (*this->side_up)[1][0].side_destination);
        }
        else if(side2 == Side::Down){
            return std::make_pair((*this->side_left)[CUBE_SIZE_3 - 1][1].side_destination, (*this->side_down)[1][0].side_destination);
        }
    }
    else if(side1 == Side::Right){
        if(side2 == Side::Front){
            return std::make_pair((*this->side_right)[1][0].side_destination, (*this->side_front)[1][CUBE_SIZE_3 - 1].side_destination);
        }
        else if(side2 == Side::Back){
            return std::make_pair((*this->side_right)[1][CUBE_SIZE_3 - 1].side_destination, (*this->side_back)[1][CUBE_SIZE_3 - 1].side_destination);
        }
        else if(side2 == Side::Up){
            return std::make_pair((*this->side_right)[0][1].side_destination, (*this->side_up)[1][CUBE_SIZE_3 - 1].side_destination);
        }
        else if(side2 == Side::Down){
            return std::make_pair((*this->side_right)[CUBE_SIZE_3 - 1][1].side_destination, (*this->side_down)[1][CUBE_SIZE_3 - 1].side_destination);
        }
    }
    else if((side1 == Side::Up && side2 == Side::Front) ||
            (side1 == Side::Front && side2 == Side::Up)){
        return std::make_pair((*this->side_front)[0][1].side_destination, (*this->side_up)[CUBE_SIZE_3 - 1][1].side_destination);
    }
    else if((side1 == Side::Down && side2 == Side::Front) ||
            (side1 == Side::Front && side2 == Side::Down)){
        return std::make_pair((*this->side_front)[CUBE_SIZE_3 - 1][1].side_destination, (*this->side_down)[CUBE_SIZE_3 - 1][1].side_destination);
    }
    else if((side1 == Side::Up && side2 == Side::Back) ||
            (side1 == Side::Back && side2 == Side::Up)){
        return std::make_pair((*this->side_back)[0][1].side_destination, (*this->side_up)[0][1].side_destination);
    }
    else if((side1 == Side::Down && side2 == Side::Back) ||
            (side1 == Side::Back && side2 == Side::Down)){
        return std::make_pair((*this->side_back)[CUBE_SIZE_3 - 1][1].side_destination, (*this->side_down)[0][1].side_destination);
    }
    return std::make_pair(Side::Front, Side::Front);
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_single_edge(Side side1, Side side2){
    switch(side1){
        case Side::Left:
            switch(side2){
                case Side::Left:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                case Side::Right:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                case Side::Front:
                    return this->get_edge_LF(); 
                    break;
                case Side::Back:
                    return this->get_edge_LB();
                    break;
                case Side::Up:
                    return this->get_edge_LU();
                    break;
                case Side::Down:
                    return this->get_edge_LD();
                    break;
                default:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
            }
            break;
        case Side::Right:
            switch(side2){
                case Side::Left:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                case Side::Right:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                case Side::Front:
                    return this->get_edge_RF(); 
                    break;
                case Side::Back:
                    return this->get_edge_RB();
                    break;
                case Side::Up:
                    return this->get_edge_RU();
                    break;
                case Side::Down:
                    return this->get_edge_RD();
                    break;
                default:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
            }
            break;
        case Side::Front:
            switch(side2){
                case Side::Left:
                    return reverse_edge(this->get_edge_LF());
                    break;
                case Side::Right:
                    return reverse_edge(this->get_edge_RF());
                    break;
                case Side::Front:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                case Side::Back:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                case Side::Up:
                    return this->get_edge_FU();
                    break;
                case Side::Down:
                    return this->get_edge_FD();
                    break;
                default:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
            }
            break;
        case Side::Back:
            switch(side2){
                case Side::Left:
                    return reverse_edge(this->get_edge_LB());
                    break;
                case Side::Right:
                    return reverse_edge(this->get_edge_RB());
                    break;
                case Side::Front:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                case Side::Back:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                case Side::Up:
                    return this->get_edge_BU();
                    break;
                case Side::Down:
                    return this->get_edge_BD();
                    break;
                default:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
            }
            break;
        case Side::Up:
            switch(side2){
                case Side::Left:
                    return reverse_edge(this->get_edge_LU());
                    break;
                case Side::Right:
                    return reverse_edge(this->get_edge_RU());
                    break;
                case Side::Front:
                    return reverse_edge(this->get_edge_FU());
                    break;
                case Side::Back:
                    return reverse_edge(this->get_edge_BU());
                    break;
                case Side::Up:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                case Side::Down:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                default:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
            }
            break;
        case Side::Down:
            switch(side2){
                case Side::Left:
                    return reverse_edge(this->get_edge_LD());
                    break;
                case Side::Right:
                    return reverse_edge(this->get_edge_RD());
                    break;
                case Side::Front:
                    return reverse_edge(this->get_edge_FD());
                    break;
                case Side::Back:
                    return reverse_edge(this->get_edge_BD());
                    break;
                case Side::Up:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                case Side::Down:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
                default:
                    return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
                    break;
            }
            break;
        default:
            return std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
            break;
    }
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_LU(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_left)[0][CUBE_SIZE_LAST_INDEX_3 - index - 1].side_destination, (*this->side_up)[CUBE_SIZE_LAST_INDEX_3 - index - 1][0].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_LD(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_left)[CUBE_SIZE_LAST_INDEX_3][CUBE_SIZE_LAST_INDEX_3 - index - 1].side_destination, (*this->side_down)[CUBE_SIZE_LAST_INDEX_3 - index - 1][0].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_RD(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_right)[CUBE_SIZE_LAST_INDEX_3][index + 1].side_destination, (*this->side_down)[CUBE_SIZE_LAST_INDEX_3 - index - 1][CUBE_SIZE_LAST_INDEX_3].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_RU(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_right)[0][index + 1].side_destination, (*this->side_up)[CUBE_SIZE_LAST_INDEX_3 - index - 1][CUBE_SIZE_LAST_INDEX_3].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_LB(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_left)[index + 1][0].side_destination, (*this->side_back)[index + 1][0].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_LF(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_left)[index + 1][CUBE_SIZE_LAST_INDEX_3].side_destination, (*this->side_front)[index + 1][0].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_RF(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_right)[index + 1][0].side_destination, (*this->side_front)[index + 1][CUBE_SIZE_LAST_INDEX_3].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_RB(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_right)[index + 1][CUBE_SIZE_LAST_INDEX_3].side_destination, (*this->side_back)[index + 1][CUBE_SIZE_LAST_INDEX_3].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_FU(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_front)[0][index + 1].side_destination, (*this->side_up)[CUBE_SIZE_LAST_INDEX_3][index + 1].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_FD(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_front)[CUBE_SIZE_LAST_INDEX_3][index + 1].side_destination, (*this->side_down)[CUBE_SIZE_LAST_INDEX_3][index + 1].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_BD(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_back)[CUBE_SIZE_LAST_INDEX_3][index + 1].side_destination, (*this->side_down)[0][index + 1].side_destination);
    }
    return edge;
}

std::array<std::pair<Side, Side>, EDGE_LENGTH_3> BasicCube3::get_edge_BU(){
    auto edge = std::array<std::pair<Side, Side>, EDGE_LENGTH_3>();
    for(size_t index = 0; index < EDGE_LENGTH_3; index++){
        edge[index] = std::make_pair((*this->side_back)[0][index + 1].side_destination, (*this->side_up)[0][index + 1].side_destination);
    }
    return edge;
}

static std::array<std::pair<Side, Side>, EDGE_LENGTH_3> reverse_edge(std::array<std::pair<Side, Side>, EDGE_LENGTH_3> edge){
    for(size_t i = 0; i < EDGE_LENGTH_3; i++){
        edge[i] = std::make_pair(std::get<1>(edge[i]), std::get<0>(edge[i]));
    }
    return edge;
}