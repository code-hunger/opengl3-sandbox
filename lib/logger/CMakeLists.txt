include_directories(.)

add_library(logger src/logger.cpp)

set_property(TARGET game PROPERTY CXX_STANDARD 11)
set_property(TARGET game PROPERTY CXX_STANDARD_REQUIRED ON)

