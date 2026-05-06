cmake_minimum_required(VERSION 3.20)

project(CppEngineLabs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(01_game_loop
    labs/01_game_loop/main.cpp
)

add_executable(02_entity_update
    labs/02_entity_update/main.cpp
)

add_executable(03_state_machine
    labs/03_state_machine/main.cpp
)

add_executable(04_input_commands 
    labs/04_input_commands/main.cpp
)

add_executable(05_functions_and_separation
    labs/05_functions_and_separation/main.cpp
)

add_executable(06_multiple_entities
    labs/06_multiple_entities/main.cpp
)

add_executable(07_delta_time
    labs/07_delta_time/main.cpp
)

add_executable(08_fixed_timestep 
    labs/08_fixed_timestep/main.cpp
)

add_executable(09_bounds_and_constraints
    labs/09_bounds_and_constraints/main.cpp
)

add_executable(10_debug_output
    labs/10_debug_output/main.cpp
)




