add_library(game_core)

target_sources(
    game_core
    PRIVATE
    game_core.c
)

target_include_directories(
    game_core
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(game_core PUBLIC linked_list)
set_source_files_properties(game_core.c PROPERTIES LANGUAGE C)
