#include "../inc/basic_cube_tile.hpp"


BasicCubeTile::BasicCubeTile(Side side_destination){
    BasicCubeTile::side_destination = side_destination;
}

BasicCubeTile::BasicCubeTile(){
    this->side_destination = Side::Error;
}