#include "../inc/cube_tile.hpp"


CubeTile::CubeTile(uint8_t x_destination,
                    uint8_t y_destination,
                    side side_current,
                    side side_destination,
                    uint8_t orientation_current,
                    uint8_t orientation_destination){
    CubeTile::x_destination = x_destination;
    CubeTile::y_destination = y_destination;
    CubeTile::side_current = side_current;
    CubeTile::side_destination = side_destination;
    CubeTile::orientation_current = orientation_current;
    CubeTile::orientation_destination = orientation_destination;
}