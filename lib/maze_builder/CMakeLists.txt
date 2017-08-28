include_directories(.)

add_library(maze_builder src/builder.cpp)

set_property(TARGET maze_builder PROPERTY CXX_STANDARD 17)
set_property(TARGET maze_builder PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(maze_builder logger)
