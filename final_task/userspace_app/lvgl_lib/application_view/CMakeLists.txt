add_library(lvgl_example example1.cpp)
target_include_directories(lvgl_example PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(lvgl_example PUBLIC lvgl_lib)
