#include "../inc/basic_cube.hpp"
#include <array>

bool compare_edge(std::pair<Side, Side> pair1, std::pair<Side, Side> pair2){
    return (pair1 == pair2) || (pair1 == std::make_pair(std::get<1>(pair2), std::get<0>(pair2)));
}

bool compare_corner(std::tuple<Side, Side, Side> tuple1, std::tuple<Side, Side, Side> tuple2){
    if(!(std::get<0>(tuple1) == std::get<0>(tuple2) ||
        std::get<0>(tuple1) == std::get<1>(tuple2) ||
        std::get<0>(tuple1) == std::get<2>(tuple2))){
        return false;
    }
    if(!(std::get<1>(tuple1) == std::get<0>(tuple2) ||
        std::get<1>(tuple1) == std::get<1>(tuple2) ||
        std::get<1>(tuple1) == std::get<2>(tuple2))){
        return false;
    }
    if(!(std::get<2>(tuple1) == std::get<0>(tuple2) ||
        std::get<2>(tuple1) == std::get<1>(tuple2) ||
        std::get<2>(tuple1) == std::get<2>(tuple2))){
        return false;
    }
    return true;
}