import sys

def write_top(file):
    file.write("/**\n")
    file.write(" *  GENERATED FILE DO NOT EDIT!\n")
    file.write(" *  CONTAINS MACRO OPTIONS FOR COMPILATION\n")
    file.write(" *  ONLY UPDATED THROUGH MAKEFILE\n")
    file.write(" */\n\n\n")
    return

def write_cube_size(file, cube_size):
    # file.write("// DEFINE YOUR CUBE SIZE (N IN NxNxN)\n")
    file.write("#ifndef CUBE_SIZE\n\n")
    file.write(f"#define CUBE_SIZE {cube_size}\n\n")
    file.write("#endif\n\n\n")

def write_cube_print(file, cube_print):
    # file.write("// COMMENT THE LINE BELOW IF YOU WANT TO BE ABLE TO PRINT CUBES\n")
    if(not cube_print):
        file.write("// ")

    file.write("#define NO_CUBE_PRINT\n\n\n")

def write_no_log(file, no_log):
    # file.write("// COMMENT THE LINE BELOW IF YOU WANT TO SEE SOME OF THE LOGS\n")
    if(not no_log):
        file.write("// ")
    file.write("#define NO_LOG\n\n\n")


def main():
    cube_size = sys.argv[1]
    cube_print = sys.argv[2]
    no_log = sys.argv[3]

    with open("config.hpp", "w") as config_file:
        write_top(config_file)
        write_cube_size(config_file, cube_size)
        write_cube_print(config_file, cube_print)
        write_no_log(config_file, no_log)
    return

main()