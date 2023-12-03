// #include "../inc/picture_cube.hpp"
// #include <cstdint>

// static inline int check_solution_side(std::vector<std::vector<PictureCubeTile*>*>* current_side, size_t cube_size);

// PictureCube::PictureCube(size_t size, CubeType type){
//     PictureCube::size = size;
//     PictureCube::type = type;
//     side_front = initialize_side(Side::Front);
//     side_back = initialize_side(Side::Back);
//     side_left = initialize_side(Side::Left);
//     side_right = initialize_side(Side::Right);
//     side_top = initialize_side(Side::Top);
//     side_bottom = initialize_side(Side::Bottom);
// }

// std::vector<std::vector<PictureCubeTile*>*>* PictureCube::initialize_side(Side side){
//     std::vector<std::vector<PictureCubeTile*>*>* new_side = new std::vector<std::vector<PictureCubeTile*>*>;
//     for(size_t i = 0; i < size; i++){
//         new_side->push_back(new std::vector<PictureCubeTile*>);
//         for(size_t j = 0; j < size; j++){
//             (*new_side)[i]->push_back(
//                                 new PictureCubeTile(i,
//                                                 j,
//                                                 side,
//                                                 side,
//                                                 Orientation::North,
//                                                 Orientation::North));
//         }
//     }
//     return new_side;
// }

// int PictureCube::rotate(Axis axis, size_t tile_index){
//     switch(axis){
//         case Axis::X:
//             if(tile_index == 0){
//                 // rotate one whole side
//             }
//             else if(tile_index == PictureCube::size - 1){
//                 // rotate one whole side
//             }
//             else{
//                 auto front_row = (*PictureCube::side_front)[tile_index];
//                 auto right_row = (*PictureCube::side_right)[tile_index];
//                 auto back_row = (*PictureCube::side_back)[tile_index];
//                 auto left_row = (*PictureCube::side_left)[tile_index];
//             }
//             return 0;
//         case Axis::Y:
//             if(tile_index == 0){
//                 // rotate one whole side
//             }
//             else if(tile_index == PictureCube::size - 1){
//                 // rotate one whole side
//             }
//             else{
                
//             }
//             return 0;
//         case Axis::Z:
//             if(tile_index == 0){
//                 // rotate one whole side
//             }
//             else if(tile_index == PictureCube::size - 1){
//                 // rotate one whole side
//             }
//             else{
                
//             }
//             return 0;
//         default:
//             return 1; // unknown enum 
//     }
// }

// int PictureCube::check_solution(){
//     int result = check_solution_side(PictureCube::side_front, PictureCube::size);
//     if(result != 0){
//         return result;
//     }
//     result = check_solution_side(PictureCube::side_back, PictureCube::size);
//     if(result != 0){
//         return result;
//     }
//     result = check_solution_side(PictureCube::side_left, PictureCube::size);
//     if(result != 0){
//         return result;
//     }
//     result = check_solution_side(PictureCube::side_right, PictureCube::size);
//     if(result != 0){
//         return result;
//     }
//     result = check_solution_side(PictureCube::side_top, PictureCube::size);
//     if(result != 0){
//         return result;
//     }
//     result = check_solution_side(PictureCube::side_bottom, PictureCube::size);
//     if(result != 0){
//         return result;
//     }
//     return 0;
// }

// static inline int check_solution_side(std::vector<std::vector<PictureCubeTile*>*>* current_side, size_t cube_size){
//     for(size_t i = 0; i < cube_size; i++){
//         for(size_t j = 0; j < cube_size; j++){
//             if((*(*current_side)[i])[j]->side_current != (*(*current_side)[i])[j]->side_destination ||
//                 (*(*current_side)[i])[j]->orientation_current != (*(*current_side)[i])[j]->orientation_destination ||
//                 i != (*(*current_side)[i])[j]->x_destination ||
//                 j != (*(*current_side)[i])[j]->y_destination){
//                 return -1;
//             }
//         }
//     }
//     return 0;
// }
