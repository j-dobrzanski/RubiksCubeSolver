
basic_cube_files := source/basic_cube_tile.cpp source/basic_cube.cpp source/basic_cube_edge_getters.cpp source/basic_cube_helpers.cpp
move_files := source/move.cpp
thistlethwaite_files := source/thistlethwaite.cpp
main_file := source/main.cpp
n_cube_solver_files := source/n_cube_solver/n_cube_solver.cpp source/n_cube_solver/n_cube_solver_reduce.cpp source/n_cube_solver/n_cube_solver_reduce_steps_selection.cpp source/n_cube_solver/n_cube_solver_reduce_steps_completion.cpp source/n_cube_solver/n_cube_solver_commutator_reduce.cpp
basic_cube_centres_subgroups_files := source/basic_cube_centres_subgroups/basic_cube_centres_subgroups.cpp
basic_cube_edges_subgroups_files := source/basic_cube_edges_subgroups/basic_cube_edges_subgroups.cpp

make:
	g++ $(basic_cube_files) $(move_files) $(thistlethwaite_files) $(n_cube_solver_files) $(basic_cube_centres_subgroups_files) $(basic_cube_edges_subgroups_files) $(main_file) -o main -ggdb3 -Wall -Wextra -O3
