cmake_minimum_required(VERSION 3.0)
project (jk)

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdouble-promotion \\\
-Wconversion -Wsign-conversion -Wlogical-op -Wabi  -Wctor-dtor-privacy \\\
-Wnon-virtual-dtor  -Wreorder \\\
-Weffc++  -Wstrict-null-sentinel \\\
-Wno-non-template-friend \\\
-Wold-style-cast \\\
-Woverloaded-virtual \\\
-Wno-pmf-conversions \\\
-Wsign-promo \\\
-Wno-error=unused-variable")

include_directories(lib)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(jk ${SOURCES})
target_link_libraries (jk GL GLU glfw GLEW)

