include_directories(include)

add_library(maze_builder builder.cpp)

target_link_libraries(maze_builder logger)
