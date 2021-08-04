  
cmake_minimum_required(VERSION 3.14)

project(Dice C CXX ASM) 

add_subdirectory(linked_list)

add_executable(dice-game "main.c")

target_link_libraries(
    dice-game
    PUBLIC
    linked_list
)

set_target_properties(
    dice-game
    PROPERTIES
    LINKER_LANGUAGE CXX
)