cmake_minimum_required(VERSION 3.0)
project (game)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdouble-promotion -Wconversion -Wsign-conversion -Wlogical-op -Wabi  -Wctor-dtor-privacy -Wmaybe-uninitialized -Wuninitialized -Wnon-virtual-dtor  -Wreorder -Weffc++  -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Woverloaded-virtual -Wno-pmf-conversions -Wsign-promo -Wno-error=unused-variable")

set(SHADER_DIRECTORY ${PROJECT_SOURCE_DIR}/src/shaders)
set(MAZE_DIRECTORY ${PROJECT_SOURCE_DIR}/data/mazes)

configure_file (
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_SOURCE_DIR}/lib/graphics/config.h"
  )

include_directories(${PROJECT_SOURCE_DIR}/lib/)
include_directories(${PROJECT_SOURCE_DIR}/lib/math/include)
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party/tclap-1.2.1/include)

file(GLOB SOURCES_RECURSIVE "src/**/*.cpp")
file(GLOB SOURCES "src/*.cpp")

add_executable(game ${SOURCES_RECURSIVE} ${SOURCES})
target_link_libraries (game graphics maze_builder)

add_subdirectory(lib/graphics)
add_subdirectory(lib/maze_builder)
add_subdirectory(lib/logger)

