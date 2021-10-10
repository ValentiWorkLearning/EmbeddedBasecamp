add_library(reminders_model reminders_model.h reminders_model.c)
target_include_directories(reminders_model PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(reminders_model PUBLIC linked_list)
set_source_files_properties(reminders_model.c reminders_model.h LANGUAGE C)
