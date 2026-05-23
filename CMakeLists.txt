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

add_executable(11_stack_vs_heap
    labs/11_stack_vs_heap/main.cpp
)

add_executable(12_references_vs_pointers
    labs/12_references_vs_pointers/main.cpp
)

add_executable(13_const_correctness
    labs/13_const_correctness/main.cpp
)

add_executable(14_raii
    labs/14_raii/main.cpp
)

add_executable(15_constructors_destructors
    labs/15_constructors_destructors/main.cpp
)

add_executable(16_unique_ptr
    labs/16_unique_ptr/main.cpp
)

add_executable(17_shared_ptr_weak_ptr
    labs/17_shared_ptr_weak_ptr/main.cpp
)

add_executable(18_copy_vs_reference_semantics
    labs/18_copy_vs_reference_semantics/main.cpp
)

add_executable(19_move_semantics
    labs/19_move_semantics/main.cpp
)



