file(GLOB SOURCES "*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/lib/include/maze_builder)

add_library(maze_builder ${SOURCES})

#target_link_libraries(maze_builder graphics)
