#include "../inc/picture_cube_tile.hpp"


PictureCubeTile::PictureCubeTile(uint8_t x_destination,
                    uint8_t y_destination,
                    Side side_current,
                    Side side_destination,
                    Orientation orientation_current,
                    Orientation orientation_destination){
    PictureCubeTile::x_destination = x_destination;
    PictureCubeTile::y_destination = y_destination;
    PictureCubeTile::side_current = side_current;
    PictureCubeTile::side_destination = side_destination;
    PictureCubeTile::orientation_current = orientation_current;
    PictureCubeTile::orientation_destination = orientation_destination;
}