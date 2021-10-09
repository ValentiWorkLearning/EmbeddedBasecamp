cmake_minimum_required(VERSION 3.20)
project(cmake_test)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 11)

add_subdirectory(linked_list)
add_subdirectory(reminders_model)
add_executable(reminders_app main.c)

target_link_libraries(reminders_app PRIVATE reminders_model)
