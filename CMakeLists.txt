cmake_minimum_required(VERSION 3.0)
project (game)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdouble-promotion -Wconversion -Wsign-conversion -Wlogical-op -Wabi  -Wctor-dtor-privacy -Wnon-virtual-dtor  -Wreorder -Weffc++  -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Woverloaded-virtual -Wno-pmf-conversions -Wsign-promo -Wno-error=unused-variable")

set(SHADER_DIRECTORY ${PROJECT_SOURCE_DIR}/src/shaders)

configure_file (
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_SOURCE_DIR}/lib/framework/config.h"
  )

include_directories(${PROJECT_SOURCE_DIR}/lib/framework)

add_executable(game src/main.cpp src/Game.cpp)
target_link_libraries (game Framework GL GLU glfw GLEW)

add_subdirectory(lib/framework)

