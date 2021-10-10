add_subdirectory(linked_list)
add_subdirectory(reminders_model)

add_library(lvgl_example example.cpp)
target_include_directories(lvgl_example PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(lvgl_example PUBLIC lvgl_lib reminders_model UtilsLibrary)
