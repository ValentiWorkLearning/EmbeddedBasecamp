cmake_minimum_required(VERSION 3.14)

project(final-lvgl C CXX ASM)

option(DESKTOP_BUILD "Use the desktop build for the application. Graphics stack-SDL" ON)

set (CONAN_DISABLE_CHECK_COMPILER TRUE)

if(DESKTOP_BUILD)
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_configure(REQUIRES sdl/2.0.16
                      GENERATORS cmake
                      IMPORTS "bin, *.dll -> ./bin"
                      IMPORTS "lib, *.dylib* -> ./bin"
                      IMPORTS "lib, *.so* -> ./bin"
                      IMPORTS "lib, *.dll -> ./bin"
)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
endif()

add_subdirectory(utils)
add_subdirectory(logger)
add_subdirectory(lvgl_lib)


set (APP_TARGET UserspaceApp)
add_executable(${APP_TARGET} ${PROJECT_SOURCES})

target_sources(
    ${APP_TARGET}
    PRIVATE
    main.cpp
    ap_application.cpp
)

target_link_libraries(
    ${APP_TARGET}
    PRIVATE
    logger_service
    graphics_backend
    CONAN_PKG::sdl

)

target_compile_features(
    ${APP_TARGET}
    PRIVATE
    cxx_std_17
)
