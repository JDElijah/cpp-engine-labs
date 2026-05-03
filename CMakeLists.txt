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