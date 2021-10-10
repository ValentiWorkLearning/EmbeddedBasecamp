cmake_minimum_required(VERSION 3.16)
project(display_test C CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(app_test main.cpp)

target_compile_features(
    app_test
    PRIVATE
    cxx_std_17
)
target_link_libraries(
	app_test
	PRIVATE
	stdc++fs
)
