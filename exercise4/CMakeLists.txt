cmake_minimum_required(VERSION 3.20)

project(game C)

add_subdirectory(game)
add_subdirectory(linked_list)

add_executable(game_app main.c)

target_link_libraries(game_app PRIVATE game_core)
set_source_files_properties(main.c PROPERTIES LANGUAGE C)
