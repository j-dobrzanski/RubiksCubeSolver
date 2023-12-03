#include "../../inc/basic_cube_subgroups/basic_cube_edges_subgroups.hpp"
#include <iostream>
#include <utility>

static std::map<std::pair<Side, Side>, size_t> orientation_map = {
    {std::make_pair(Side::Left, Side::Up), 0},
    {std::make_pair(Side::Left, Side::Down), 1},
    {std::make_pair(Side::Right, Side::Down), 1},
    {std::make_pair(Side::Right, Side::Up), 0},
    {std::make_pair(Side::Left, Side::Back), 1},
    {std::make_pair(Side::Left, Side::Front), 0},
    {std::make_pair(Side::Right, Side::Front), 1},
    {std::make_pair(Side::Right, Side::Back), 0},
    {std::make_pair(Side::Front, Side::Up), 0},
    {std::make_pair(Side::Front, Side::Down), 1},
    {std::make_pair(Side::Back, Side::Down), 0},
    {std::make_pair(Side::Back, Side::Up), 1}
};

static std::map<std::pair<Side, Side>, std::pair<Side, Side>> commutator_map = {
    {std::make_pair(Side::Left, Side::Up), std::make_pair(Side::Back, Side::Front)},
    {std::make_pair(Side::Left, Side::Down), std::make_pair(Side::Front, Side::Back)},
    {std::make_pair(Side::Right, Side::Down), std::make_pair(Side::Front, Side::Back)},
    {std::make_pair(Side::Right, Side::Up), std::make_pair(Side::Back, Side::Front)},
    {std::make_pair(Side::Left, Side::Back), std::make_pair(Side::Down, Side::Up)},
    {std::make_pair(Side::Left, Side::Front), std::make_pair(Side::Up, Side::Down)},
    {std::make_pair(Side::Right, Side::Front), std::make_pair(Side::Up, Side::Down)},
    {std::make_pair(Side::Right, Side::Back), std::make_pair(Side::Down, Side::Up)},
    {std::make_pair(Side::Front, Side::Up), std::make_pair(Side::Left, Side::Right)},
    {std::make_pair(Side::Front, Side::Down), std::make_pair(Side::Right, Side::Left)},
    {std::make_pair(Side::Back, Side::Down), std::make_pair(Side::Right, Side::Left)},
    {std::make_pair(Side::Back, Side::Up), std::make_pair(Side::Left, Side::Right)}
};

static std::map<Side, Side> opposite_sides = {
    {Side::Left, Side::Right},
    {Side::Right, Side::Left},
    {Side::Front, Side::Back},
    {Side::Back, Side::Front},
    {Side::Up, Side::Down},
    {Side::Down, Side::Up},
};


BasicCubeEdgesSubgroup::BasicCubeEdgesSubgroup(BasicCube* cube, std::pair<Side, Side> edge, size_t index, bool is_singular){
    this->no_of_misplaced_edge_parts = 0;
    this->base_edge = edge;
    this->base_index = index;
    this->is_singular = is_singular;

    this->subgroup = this->generate_subgroup(cube);
}

BasicCubeEdgesSubgroup::BasicCubeEdgesSubgroup(BasicCubeEdgesSubgroup* subgroup){
    this->no_of_misplaced_edge_parts = subgroup->no_of_misplaced_edge_parts;
    this->base_edge = subgroup->base_edge;
    this->base_index = subgroup->base_index;

    // TODO: do later
}

int BasicCubeEdgesSubgroup::get_position(std::pair<Side, Side> edge, size_t subgroup_index, size_t* row_index_side1, size_t* column_index_side1, size_t* row_index_side2, size_t* column_index_side2){
    if(compare_edge(edge, std::make_pair(Side::Left, Side::Front))){
        if(subgroup_index == 0){
            *row_index_side1 = this->base_index;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX;
            *row_index_side2 = this->base_index;
            *column_index_side2 = 0;
            return 0;
        }
        else{
            *row_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side2 = 0;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Left, Side::Back))){
        if(subgroup_index == 0){
            *row_index_side1 = this->base_index;
            *column_index_side1 = 0;
            *row_index_side2 = this->base_index;
            *column_index_side2 = 0;
            return 0;
        }
        else{
            *row_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side1 = 0;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side2 = 0;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Left, Side::Up))){
        if(subgroup_index == 0){
            *row_index_side1 = 0;
            *column_index_side1 = this->base_index;
            *row_index_side2 = this->base_index;
            *column_index_side2 = 0;
            return 0;
        }
        else{
            *row_index_side1 = 0;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side2 = 0;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Left, Side::Down))){
        if(subgroup_index == 0){
            *row_index_side1 = CUBE_SIZE_LAST_INDEX;
            *column_index_side1 = this->base_index;
            *row_index_side2 = this->base_index;
            *column_index_side2 = 0;
            return 0;
        }
        else{
            *row_index_side1 = CUBE_SIZE_LAST_INDEX;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side2 = 0;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Right, Side::Front))){
        if(subgroup_index == 0){
            *row_index_side1 = this->base_index;
            *column_index_side1 = 0;
            *row_index_side2 = this->base_index;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX;
            return 0;
        }
        else{
            *row_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side1 = 0;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Right, Side::Back))){
        if(subgroup_index == 0){
            *row_index_side1 = this->base_index;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX;
            *row_index_side2 = this->base_index;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX;
            return 0;
        }
        else{
            *row_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Right, Side::Up))){
        if(subgroup_index == 0){
            *row_index_side1 = 0;
            *column_index_side1 = this->base_index;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX;
            return 0;
        }
        else{
            *row_index_side1 = 0;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *row_index_side2 = this->base_index;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Right, Side::Down))){
        if(subgroup_index == 0){
            *row_index_side1 = CUBE_SIZE_LAST_INDEX;
            *column_index_side1 = this->base_index;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX;
            return 0;
        }
        else{
            *row_index_side1 = CUBE_SIZE_LAST_INDEX;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *row_index_side2 = this->base_index;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Front, Side::Up))){
        if(subgroup_index == 0){
            *row_index_side1 = 0;
            *column_index_side1 = this->base_index;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX;
            *column_index_side2 = this->base_index;
            return 0;
        }
        else{
            *row_index_side1 = 0;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Front, Side::Down))){
        if(subgroup_index == 0){
            *row_index_side1 = CUBE_SIZE_LAST_INDEX;
            *column_index_side1 = this->base_index;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX;
            *column_index_side2 = this->base_index;
            return 0;
        }
        else{
            *row_index_side1 = CUBE_SIZE_LAST_INDEX;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *row_index_side2 = CUBE_SIZE_LAST_INDEX;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Back, Side::Up))){
        if(subgroup_index == 0){
            *row_index_side1 = 0;
            *column_index_side1 = this->base_index;
            *row_index_side2 = 0;
            *column_index_side2 = this->base_index;
            return 0;
        }
        else{
            *row_index_side1 = 0;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *row_index_side2 = 0;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            return 0;
        }
    }
    else if(compare_edge(edge, std::make_pair(Side::Back, Side::Down))){
        if(subgroup_index == 0){
            *row_index_side1 = CUBE_SIZE_LAST_INDEX;
            *column_index_side1 = this->base_index;
            *row_index_side2 = 0;
            *column_index_side2 = this->base_index;
            return 0;
        }
        else{
            *row_index_side1 = CUBE_SIZE_LAST_INDEX;
            *column_index_side1 = CUBE_SIZE_LAST_INDEX - this->base_index;
            *row_index_side2 = 0;
            *column_index_side2 = CUBE_SIZE_LAST_INDEX - this->base_index;
            return 0;
        }
    }
    return 1;
}

std::map<std::pair<Side, Side>, std::array<std::pair<Side, Side>, 2>> BasicCubeEdgesSubgroup::generate_subgroup(BasicCube* base_cube){
    std::map<std::pair<Side, Side>, std::array<std::pair<Side, Side>, 2>> generated_subgroup = std::map<std::pair<Side, Side>, std::array<std::pair<Side, Side>, 2>>();

    for(auto edge_part : ordered_edge_set){
        size_t row_index_side1 = 0;
        size_t column_index_side1 = 0;
        size_t row_index_side2 = 0;
        size_t column_index_side2 = 0;
        this->get_position(edge_part, 0, &row_index_side1, &column_index_side1, &row_index_side2, &column_index_side2);
        auto found_edge_part = std::make_pair((*base_cube->sides[edge_part.first])[row_index_side1][column_index_side1].side_destination, (*base_cube->sides[edge_part.second])[row_index_side2][column_index_side2].side_destination);
        generated_subgroup[edge_part][0] = found_edge_part;


        this->no_of_misplaced_edge_parts += !compare_edge(edge_part, found_edge_part);

        this->get_position(edge_part, 1, &row_index_side1, &column_index_side1, &row_index_side2, &column_index_side2);
        found_edge_part = std::make_pair((*base_cube->sides[edge_part.first])[row_index_side1][column_index_side1].side_destination, (*base_cube->sides[edge_part.second])[row_index_side2][column_index_side2].side_destination);
        generated_subgroup[edge_part][1] = found_edge_part;

        this->no_of_misplaced_edge_parts += !compare_edge(edge_part, found_edge_part);

    }
    // std::cout << "Generated subgroup: " << std::endl;
    // for(auto side : generated_subgroup){
    //     std::cout << side_to_char(side.first.first) << ", " << side_to_char(side.first.second) << std::endl;
    //     std::cout << "\t0. " << side_to_char(side.second[0].first) << ", " << side_to_char(side.second[0].second) << std::endl;
    //     std::cout << "\t1. " << side_to_char(side.second[1].first) << ", " << side_to_char(side.second[1].second) << std::endl;
    // }

    return generated_subgroup;
}

int BasicCubeEdgesSubgroup::naive_solve(){
    // std::cout << "No of misplaced edges: " << this->no_of_misplaced_edge_parts << std::endl;
    while(this->no_of_misplaced_edge_parts > 0){
        auto old = this->no_of_misplaced_edge_parts;

        for(auto edge : this->subgroup){
            for(size_t i = 0; i < 2; i++){
                if(!compare_edge(edge.first, edge.second[i])){ // found misplaced tile
                    // std::cout << "##################################" << std::endl;
                    // std::cout << "Found misplaced edge: (" << side_to_char(edge.first.first) << ", " << side_to_char(edge.first.second) << ") <-> (";
                    // std::cout << side_to_char(edge.second[i].first) << ", " << side_to_char(edge.second[i].second) << ") on index " << i << std::endl;
                    // this->print_subgroup();
                    auto destination = edge.second[i];
                    for(size_t j = 0; j < 2; j++){
                        // this->print_subgroup();
                        if(!compare_edge(destination, this->subgroup[normalize_edge(destination)][j])){ // found free space for tile
                            swap_path.push_back(std::make_pair(std::make_pair(normalize_edge(edge.first), i), std::make_pair(normalize_edge(destination), j)));

                            // std::cout << "Before swap: " << std::endl;
                            // this->print_subgroup();
                            auto temp = edge.second[i];
                            this->subgroup[normalize_edge(edge.first)][i] = this->subgroup[normalize_edge(destination)][j];
                            this->subgroup[normalize_edge(destination)][j] = temp;

                            if(is_singular){
                                this->subgroup[normalize_edge(edge.first)][1 - i] = this->subgroup[normalize_edge(destination)][1 - j];
                                this->subgroup[normalize_edge(destination)][1 - j] = temp;
                            }

                            // std::cout << "After swap: " << std::endl;
                            // this->print_subgroup();
                            // std::cout << "##################################" << std::endl;

                            if(compare_edge(this->subgroup[normalize_edge(edge.first)][i], edge.first)){
                                this->no_of_misplaced_edge_parts--;
                            }
                            this->no_of_misplaced_edge_parts--;
                            break;
                        }
                    }
                } // tiles swapped
            }
        }
        // this->print_subgroup();
        if(old == this->no_of_misplaced_edge_parts){
#ifndef NO_LOG
            std::cout << "no swap found" << std::endl;
#endif
            return 1;
        }
    }
    return 0;
}

int BasicCubeEdgesSubgroup::apply_to_cube(BasicCube *cube){
    for(auto swap : this->swap_path){
        size_t first_row_index_side1 = 0;
        size_t first_row_index_side2 = 0;
        size_t first_column_index_side1 = 0;
        size_t first_column_index_side2 = 0;

        size_t second_row_index_side1 = 0;
        size_t second_row_index_side2 = 0;
        size_t second_column_index_side1 = 0;
        size_t second_column_index_side2 = 0;

        // std::cout << "Performing swap for: " << std::endl;

        // std::cout << side_to_char(swap.first.first.first) << ", " << side_to_char(swap.first.first.second) << " : " << swap.first.second << std::endl;
        this->get_position(swap.first.first, swap.first.second, &first_row_index_side1, &first_column_index_side1, &first_row_index_side2, &first_column_index_side2);
        // std::cout << "\t" << side_to_char(swap.first.first.first) << ": " << first_row_index_side1 << ", " << first_column_index_side1 << std::endl; 
        // std::cout << "\t" << side_to_char(swap.first.first.second) << ": " << first_row_index_side2 << ", " << first_column_index_side2 << std::endl; 

        // std::cout << side_to_char(swap.second.first.first) << ", " << side_to_char(swap.second.first.second) << " : " << swap.second.second << std::endl;
        this->get_position(swap.second.first, swap.second.second, &second_row_index_side1, &second_column_index_side1, &second_row_index_side2, &second_column_index_side2);
        // std::cout << "\t" << side_to_char(swap.second.first.first) << ": " << second_row_index_side1 << ", " << second_column_index_side1 << std::endl; 
        // std::cout << "\t" << side_to_char(swap.second.first.second) << ": " << second_row_index_side2 << ", " << second_column_index_side2 << std::endl; 


        if((orientation_map[swap.first.first] == swap.first.second) == 
            (orientation_map[swap.second.first] == swap.second.second)){
            auto temp = (*cube->sides[swap.first.first.first])[first_row_index_side1][first_column_index_side1];
            (*cube->sides[swap.first.first.first])[first_row_index_side1][first_column_index_side1] = (*cube->sides[swap.second.first.first])[second_row_index_side1][second_column_index_side1];
            (*cube->sides[swap.second.first.first])[second_row_index_side1][second_column_index_side1] = temp;

            temp = (*cube->sides[swap.first.first.second])[first_row_index_side2][first_column_index_side2];
            (*cube->sides[swap.first.first.second])[first_row_index_side2][first_column_index_side2] = (*cube->sides[swap.second.first.second])[second_row_index_side2][second_column_index_side2];
            (*cube->sides[swap.second.first.second])[second_row_index_side2][second_column_index_side2] = temp;
        }
        else{
            auto temp = (*cube->sides[swap.first.first.first])[first_row_index_side1][first_column_index_side1];
            (*cube->sides[swap.first.first.first])[first_row_index_side1][first_column_index_side1] = (*cube->sides[swap.second.first.second])[second_row_index_side2][second_column_index_side2];
            (*cube->sides[swap.second.first.second])[second_row_index_side2][second_column_index_side2] = temp;

            temp = (*cube->sides[swap.first.first.second])[first_row_index_side2][first_column_index_side2];
            (*cube->sides[swap.first.first.second])[first_row_index_side2][first_column_index_side2] = (*cube->sides[swap.second.first.first])[second_row_index_side1][second_column_index_side1];
            (*cube->sides[swap.second.first.first])[second_row_index_side1][second_column_index_side1] = temp;
        }
        // cube->print_cube();
    }
    return 0;
}

int BasicCubeEdgesSubgroup::print_subgroup(){
    std::cout << "Generated subgroup: " << std::endl;
    for(auto side : this->subgroup){
        std::cout << side_to_char(side.first.first) << ", " << side_to_char(side.first.second) << std::endl;
        std::cout << "\t0. " << side_to_char(side.second[0].first) << ", " << side_to_char(side.second[0].second) << std::endl;
        std::cout << "\t1. " << side_to_char(side.second[1].first) << ", " << side_to_char(side.second[1].second) << std::endl;
    }
    return 0;
}

std::vector<Move*> generate_commutator(std::pair<Side, Side> edge1, size_t subgroup_index1, std::pair<Side, Side> edge2, size_t subgroup_index2){
    std::vector<Move*> moves = std::vector<Move*>();
    std::vector<Move*> append_moves = std::vector<Move*>();

    Side pseudo_left = commutator_map[edge1].first;
    Side pseudo_right = commutator_map[edge1].second;
    Side pseudo_up = edge1.second;
    Side pseudo_front = edge1.first;
    Side pseudo_back = opposite_sides[pseudo_front];
    Side pseudo_down = opposite_sides[pseudo_up];

    if(compare_edge(edge2, std::make_pair(pseudo_back, pseudo_up))){
        if(subgroup_index1 == subgroup_index2){
            // basic case   
        }
        else{
            moves.push_back(new Move(pseudo_back, 0, false, false));
            moves.push_back(new Move(pseudo_right, 0, true, false));
            moves.push_back(new Move(pseudo_down, 0, true, false));
            moves.push_back(new Move(pseudo_back, 0, true, true));

            append_moves.push_back(new Move(pseudo_back, 0, true, true));
            append_moves.push_back(new Move(pseudo_down, 0, false, false));
            append_moves.push_back(new Move(pseudo_right, 0, false, false));
            append_moves.push_back(new Move(pseudo_back, 0, true, false));
        }
    }
    else if(compare_edge(edge2, std::make_pair(pseudo_back, pseudo_left))){
        if(subgroup_index1 == subgroup_index2){
            moves.push_back(new Move(pseudo_back, 0, true, true));
            moves.push_back(new Move(pseudo_right, 0, true, false));
            moves.push_back(new Move(pseudo_down, 0, true, false));
            moves.push_back(new Move(pseudo_back, 0, true, true));

            append_moves.push_back(new Move(pseudo_back, 0, true, true));
            append_moves.push_back(new Move(pseudo_down, 0, false, false));
            append_moves.push_back(new Move(pseudo_right, 0, false, false));
            append_moves.push_back(new Move(pseudo_back, 0, true, true));
        }
        else{
            moves.push_back(new Move(pseudo_back, 0, true, false));

            append_moves.push_back(new Move(pseudo_back, 0, false, false));
        }
    }
    else if(compare_edge(edge2, std::make_pair(pseudo_back, pseudo_down))){
        if(subgroup_index1 == subgroup_index2){
            moves.push_back(new Move(pseudo_back, 0, true, false));
            moves.push_back(new Move(pseudo_right, 0, true, false));
            moves.push_back(new Move(pseudo_down, 0, true, false));
            moves.push_back(new Move(pseudo_back, 0, true, true));

            append_moves.push_back(new Move(pseudo_back, 0, true, true));
            append_moves.push_back(new Move(pseudo_down, 0, false, false));
            append_moves.push_back(new Move(pseudo_right, 0, false, false));
            append_moves.push_back(new Move(pseudo_back, 0, false, false));
        }
        else{
            moves.push_back(new Move(pseudo_back, 0, false, false));

            append_moves.push_back(new Move(pseudo_back, 0, true, false));
        }
    }
    else if(compare_edge(edge2, std::make_pair(pseudo_back, pseudo_right))){
        if(subgroup_index1 == subgroup_index2){
            moves.push_back(new Move(pseudo_back, 0, true, false));

            append_moves.push_back(new Move(pseudo_back, 0, false, false));
        }
        else{
            moves.push_back(new Move(pseudo_right, 0, true, false));
            moves.push_back(new Move(pseudo_down, 0, true, false));
            moves.push_back(new Move(pseudo_back, 0, true, true));

            append_moves.push_back(new Move(pseudo_back, 0, true, true));
            append_moves.push_back(new Move(pseudo_down, 0, false, false));
            append_moves.push_back(new Move(pseudo_right, 0, false, false));
        }
    }
    else if(compare_edge(edge2, std::make_pair(pseudo_right, pseudo_up))){
        if(subgroup_index1 == subgroup_index2){
            moves.push_back(new Move(pseudo_right, 0, true, false));
            moves.push_back(new Move(pseudo_back, 0, true, false));

            append_moves.push_back(new Move(pseudo_back, 0, false, false));
            append_moves.push_back(new Move(pseudo_right, 0, false, false));
        }
        else{
            moves.push_back(new Move(pseudo_right, 0, true, true));
            moves.push_back(new Move(pseudo_down, 0, true, false));
            moves.push_back(new Move(pseudo_back, 0, true, true));

            append_moves.push_back(new Move(pseudo_back, 0, true, true));
            append_moves.push_back(new Move(pseudo_down, 0, false, false));
            append_moves.push_back(new Move(pseudo_right, 0, true, true));
        }
    }
    else if(compare_edge(edge2, std::make_pair(pseudo_right, pseudo_front))){
        if(subgroup_index1 == subgroup_index2){
            moves.push_back(new Move(pseudo_right, 0, false, false));
            moves.push_back(new Move(pseudo_down, 0, false, false));
            moves.push_back(new Move(pseudo_back, 0, true, false));

            append_moves.push_back(new Move(pseudo_back, 0, false, false));
            append_moves.push_back(new Move(pseudo_down, 0, true, false));
            append_moves.push_back(new Move(pseudo_right, 0, true, false));
        }
        else{
            moves.push_back(new Move(pseudo_right, 0, true, true));
            moves.push_back(new Move(pseudo_back, 0, true, false));

            append_moves.push_back(new Move(pseudo_back, 0, false, false));
            append_moves.push_back(new Move(pseudo_right, 0, false, true));            
        }
    }
    else if(compare_edge(edge2, std::make_pair(pseudo_right, pseudo_down))){
        if(subgroup_index1 == subgroup_index2){
            moves.push_back(new Move(pseudo_down, 0, false, false));
            moves.push_back(new Move(pseudo_back, 0, true, true));

            append_moves.push_back(new Move(pseudo_back, 0, true, false));
            append_moves.push_back(new Move(pseudo_down, 0, true, false));
        }
        else{
            moves.push_back(new Move(pseudo_right, 0, false, false));
            moves.push_back(new Move(pseudo_back, 0, true, false));

            append_moves.push_back(new Move(pseudo_back, 0, false, false));
            append_moves.push_back(new Move(pseudo_right, 0, true, false));            
        }
    }
    else if(compare_edge(edge2, std::make_pair(pseudo_left, pseudo_down))){
        if(subgroup_index1 == subgroup_index2){
            moves.push_back(new Move(pseudo_down, 0, true, false));
            moves.push_back(new Move(pseudo_back, 0, true, true));

            append_moves.push_back(new Move(pseudo_back, 0, true, true));
            append_moves.push_back(new Move(pseudo_down, 0, false, false));
        }
        else{
            moves.push_back(new Move(pseudo_left, 0, true, false));
            moves.push_back(new Move(pseudo_back, 0, false, false));

            append_moves.push_back(new Move(pseudo_back, 0, true, false));
            append_moves.push_back(new Move(pseudo_left, 0, false, false));
        }
    }
    else if(compare_edge(edge2, std::make_pair(pseudo_left, pseudo_front))){
        if(subgroup_index1 == subgroup_index2){
            moves.push_back(new Move(pseudo_left, 0, true, true));
            moves.push_back(new Move(pseudo_back, 0, false, false));

            append_moves.push_back(new Move(pseudo_back, 0, true, false));
            append_moves.push_back(new Move(pseudo_left, 0, true, true));
        }
        else{
            moves.push_back(new Move(pseudo_left, 0, false, false));
            moves.push_back(new Move(pseudo_down, 0, true, false));
            moves.push_back(new Move(pseudo_back, 0, true, false));

            append_moves.push_back(new Move(pseudo_back, 0, false, false));
            append_moves.push_back(new Move(pseudo_down, 0, false, false));
            append_moves.push_back(new Move(pseudo_left, 0, true, false));
        }
    }
    else if(compare_edge(edge2, std::make_pair(pseudo_left, pseudo_up))){
        if(subgroup_index1 == subgroup_index2){
            moves.push_back(new Move(pseudo_left, 0, false, false));
            moves.push_back(new Move(pseudo_back, 0, false, false));

            append_moves.push_back(new Move(pseudo_back, 0, true, false));
            append_moves.push_back(new Move(pseudo_left, 0, true, false));
        }
        else{
            moves.push_back(new Move(pseudo_left, 0, true, true));
            moves.push_back(new Move(pseudo_down, 0, false, false));
            moves.push_back(new Move(pseudo_back, 0, true, true));

            append_moves.push_back(new Move(pseudo_back, 0, true, true));
            append_moves.push_back(new Move(pseudo_down, 0, true, false));
            append_moves.push_back(new Move(pseudo_left, 0, true, true));
            
        }
    }
    else if(compare_edge(edge2, std::make_pair(pseudo_front, pseudo_down))){
        if(subgroup_index1 == subgroup_index2){
            moves.push_back(new Move(pseudo_down, 0, true, true));
            moves.push_back(new Move(pseudo_back, 0, true, true));

            append_moves.push_back(new Move(pseudo_back, 0, true, true));
            append_moves.push_back(new Move(pseudo_down, 0, true, true));
        }
        else{
            moves.push_back(new Move(pseudo_down, 0, true, false));
            moves.push_back(new Move(pseudo_right, 0, false, false));
            moves.push_back(new Move(pseudo_back, 0, true, false));

            append_moves.push_back(new Move(pseudo_back, 0, false, false));
            append_moves.push_back(new Move(pseudo_right, 0, true, false));
            append_moves.push_back(new Move(pseudo_down, 0, false, false));
        }
    }
    

    for(auto move: append_moves){
        moves.push_back(move);
    }

    return moves;
}


