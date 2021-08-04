add_library(linked_list INTERFACE linked_list.h)

target_include_directories(
	linked_list
	INTERFACE
	${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(
	linked_list
	PROPERTIES
	LINKER_LANGUAGE CXX
)