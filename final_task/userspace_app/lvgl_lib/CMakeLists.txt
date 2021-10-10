add_subdirectory(lvgl_library)

if(DESKTOP_BUILD)
add_subdirectory(lvgl_driver_backend)
endif()

add_subdirectory(platform)
add_subdirectory(application_view)

add_library(graphics_backend gs_lvgl_service.cpp)
target_include_directories(graphics_backend PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(graphics_backend PUBLIC graphics_platform_backend UtilsLibrary lvgl_example lvgl_lib  )

if(DESKTOP_BUILD)
    target_link_libraries(graphics_backend PUBLIC lvgl_drivers)
endif()
