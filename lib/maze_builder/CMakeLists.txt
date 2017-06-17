include_directories(.)

add_library(maze_builder src/builder.cpp)

target_link_libraries(maze_builder logger)
