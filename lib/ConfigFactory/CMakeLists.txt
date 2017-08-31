include_directories(.)

add_library(ConfigFactory src/ConfigFactory.cpp)
target_link_libraries(ConfigFactory maze_builder)

set_property(TARGET ConfigFactory PROPERTY CXX_STANDARD 17)
set_property(TARGET ConfigFactory PROPERTY CXX_STANDARD_REQUIRED ON)

