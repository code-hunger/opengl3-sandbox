file(GLOB SOURCES "*.cpp")

add_library(logging ${SOURCES})

target_link_libraries (logging pthread dl)

#target_link_libraries(maze_builder graphics)
