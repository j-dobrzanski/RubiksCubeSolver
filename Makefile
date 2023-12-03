
basic_cube_files := source/basic_cube_tile.cpp source/basic_cube.cpp source/basic_cube_edge_getters.cpp source/basic_cube_helpers.cpp source/basic_cube_3.cpp source/basic_cube_3_edge_getters.cpp
move_files := source/move.cpp
thistlethwaite_files := source/thistlethwaite.cpp
main_file := source/main.cpp
n_cube_solver_files := source/n_cube_solver/n_cube_solver.cpp source/n_cube_solver/n_cube_solver_reduce.cpp source/n_cube_solver/n_cube_solver_reduce_steps_selection.cpp source/n_cube_solver/n_cube_solver_reduce_steps_completion.cpp source/n_cube_solver/n_cube_solver_commutator_reduce.cpp
basic_cube_centres_subgroups_files := source/basic_cube_centres_subgroups/basic_cube_centres_subgroups.cpp
basic_cube_edges_subgroups_files := source/basic_cube_edges_subgroups/basic_cube_edges_subgroups.cpp

CUBE_SIZE = 3
NO_LOG = 1
CUBE_PRINT = 0
PROGRAM_NAME = main

make:
	python generate_config.py $(CUBE_SIZE) $(CUBE_PRINT) $(NO_LOG)
	g++ $(basic_cube_files) $(move_files) $(thistlethwaite_files) $(n_cube_solver_files) $(basic_cube_centres_subgroups_files) $(basic_cube_edges_subgroups_files) $(main_file) -o $(PROGRAM_NAME) -ggdb3 -Wall -Wextra -Werror -O3

help:
	@echo "Dostępne opcje kompilacji:"
	@echo "	CUBE_SIZE=int		-	rozmiar kostek w programie - def 3"
	@echo "	NO_LOG={0,1}		- 	opcja wyłącznia logów - def 1"
	@echo "	CUBE_PRINT={0,1}	-	opcja włącznia możliwości wyświetlania kostek - def 0"
	@echo "	PROGRAM_NAME=str	-	nazwa programu wynikowego - def main"