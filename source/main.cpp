#include "../inc/basic_cube.hpp"
#include <iostream>
#include "../inc/thistlethwaite.hpp"
#include "../inc/n_cube_solver/n_cube_solver.hpp"
#include <bitset>
#include <chrono>
#include <string>
#include <random>


static std::string random_move_sequence(size_t length);

// static void print_move(Move* move);

int main(int argc, char** argv){
    std::string move_sequence;
    size_t length;
    bool only_is_solved = false;
    bool show_solution = true;
    int commutator_strategy = 0;
    // bool show_stats = true;

    if(argc == 1){
        std::cout << "Wybrana konfiguracja: " << std::endl;
        std::cout << "\t rozmiar kostki: " << CUBE_SIZE << std::endl;

#ifdef NO_CUBE_PRINT
        std::cout << "\t kostki nie będą wyświetlane" << std::endl;
#else
        std::cout << "\t kostki będą wyświetlane" << std::endl;
#endif

#ifdef NO_LOG
        std::cout << "\t logi nie będą wyświetlane" << std::endl;
#else
        std::cout << "\t logi będą wyświetlane" << std::endl;
#endif
        std::cout << "Wpisz 1 aby wyświetlić rozwiązanie końcowe lub 0 aby je pominąć" << std::endl;
        std::cin >> show_solution;
        std::cout << "Wpisz 1 dla ciągu losowego lub 2 dla własnego ciągu ruchów do pomieszania kostki: " << std::endl;
        size_t option = 0;
        std::cin >> option;
        if(option == 1){
            std::cout << "Podaj liczbę ruchów do pomieszania: " << std::endl;
            std::cin >> length;
            move_sequence = random_move_sequence(length);
            std::cout << "Wygenerowana sekwencja: " << std::endl;
            std::cout << move_sequence << std::endl;            
        }
        else if(option == 2){
            std::cout << "Podaj sekwencję ruchów mieszającą kostkę: " << std::endl;
            std::cin >> move_sequence;            
        }
    }
    else{
        if(argc < 4){
            std::cout << "Zła ilość opcji" << std::endl;
            return 0;
        }
        show_solution = false;
        length = std::stoi(argv[1]);
        only_is_solved = std::stoi(argv[2]);
        commutator_strategy = std::stoi(argv[3]);
        // show_stats = std::stoi(argv[3]);
        move_sequence = random_move_sequence(length);

    }


    auto cube = new BasicCube();
    // debug();

    // cube->rotate_sequence("1U2 0D2 0B2 1D' 0U' 0L2 0R' 0F2 1L2 0R' 0U2 1F 1L2 0D2 1R' 0B 0R' 0U2 0D2 1R2 0B' 0U2 0D 0R2 0U");

    // cube->rotate_sequence("1U2 0D2 0B2 1D' 0U' 3L2 0R' 0F2 1L2 0R' 5U2 1F 1L2 3D2 1R' 7B 0R' 0U2 0D2 1R2 0B' 0U2 0D 0R2 0U 5U2 0D2 7B2 1D' 0U' 5L2 6R' 0F2 1L2 7R' 1U2 1F 1L2 0D2 1R' 2B 0R' 0U2 5D2 1R2 0B' 2U2 0D 3R2 0U  4U2 2D2 0B2 1D' 04U' 0L2 0R' 02F2 1L2 0R' 0U2 1F 1L2 3D2 1R' 0B 4R' 7U2 0D2 1R2 0B' 0U2 0D 8R2 0U ");

    cube->rotate_sequence(move_sequence);

    if(argc == 1){
        std::cout << "Wygląd pomieszanej kostki: " << std::endl;
        cube->print_cube();
    }
    cube->clear_solution_path();

    auto n_cube_solver = new NCubeSolver(cube);

    auto reducing_start = std::chrono::high_resolution_clock::now();
    int result = n_cube_solver->commutator_reduce(commutator_strategy);
    auto reducing_end = std::chrono::high_resolution_clock::now();


#ifndef NO_LOG
    if(result == 0){
        n_cube_solver->reduced_cube->print_cube();        
    }
#endif

    auto thistlethwaite = new Thistlethwaite(n_cube_solver->reduced_cube);
    auto thistlethwaite_solving_start = std::chrono::high_resolution_clock::now();
    result = thistlethwaite->solve();
    auto thistlethwaite_solving_end = std::chrono::high_resolution_clock::now();

    if(result == 0){
        // thistlethwaite->solved_cube->print_cube();
        if(!only_is_solved){
            if(show_solution){
                thistlethwaite->solved_cube->print_solution_path();                
            }
            std::cout << "Długość rozwiązania: " << thistlethwaite->solved_cube->solution_length << std::endl;
            std::cout << "Czas redukcji kostki do 3x3x3: " << std::chrono::duration_cast<std::chrono::microseconds>(reducing_end - reducing_start).count() << "us" << std::endl;
            std::cout << "Czas rozwiązywania 3x3x3: " << std::chrono::duration_cast<std::chrono::microseconds>(thistlethwaite_solving_end - thistlethwaite_solving_start).count() << "us" << std::endl;
            std::cout << "Łączny czas: " << ((double)std::chrono::duration_cast<std::chrono::microseconds>(reducing_end - reducing_start).count() + std::chrono::duration_cast<std::chrono::microseconds>(thistlethwaite_solving_end - thistlethwaite_solving_start).count())/1000000 << "s" << std::endl;
            
        }
        else{
            std::cout << 1 << std::endl;
        }
        // std::cout << "is ok: " << thistlethwaite->solved_cube->check_solution();      
    }
    else{
        if(!only_is_solved){
            std::cout << "Brak rozwiązania" << std::endl;            
        }
        else{
            std::cout << 0 << std::endl;
        }
    }


    delete thistlethwaite;    
    delete n_cube_solver;
    delete cube;
    return 0;
}

// 0L 0R 0F2 0U 0L 0U 0D 0F 0R' 0D2 0F 0B2 0L' 0F 0L 0U2 0F 0L 0F2 0L 0R 0D2 0L2 0U2 0L 0U2 0B2 0L2 0U2 0L

// static void print_move(Move* move){
//     std::cout << move->to_string() << std::endl;
// }

static std::string random_move_sequence(size_t length){
    Side sides[6] = {
        Side::Left,
        Side::Right,
        Side::Front,
        Side::Back,
        Side::Up,
        Side::Down
    };
    std::string move_sequence = "";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> side(0, 5);
    std::uniform_int_distribution<> index(0, (CUBE_SIZE_LAST_INDEX-1)/2);
    std::uniform_int_distribution<> move(0,2);
    for(size_t i = 0; i < length; i++){
        int orientation = move(gen);
        Move* new_move = new Move(sides[side(gen)], index(gen), orientation == 0, orientation == 2);
        move_sequence.append(new_move->to_string());
        move_sequence.append(" ");
        delete new_move;
    }
    return move_sequence;
}