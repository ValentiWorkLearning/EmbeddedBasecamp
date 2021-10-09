add_library(
    graphics_platform_backend
    gs_platform_layer.cpp
)

target_link_libraries(
    graphics_platform_backend
    PRIVATE
    lvgl_lib
    UtilsLibrary
    logger_service
)

if(${DESKTOP_BUILD})
    target_compile_definitions(
        graphics_platform_backend
        PRIVATE
        USE_SDL_BACKEND
    )
else()
    target_compile_definitions(
        graphics_platform_backend
        PRIVATE
        USE_HARDWARE_DISPLAY
    )

endif()
target_link_libraries(
    graphics_platform_backend
    PUBLIC
    lvgl_lib
    lvgl_drivers
)

