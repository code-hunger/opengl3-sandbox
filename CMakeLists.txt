cmake_minimum_required(VERSION 2.8.9)
project (jk)

file(GLOB SOURCES "*.cpp")

include_directories(lib/glm)

add_executable(jk ${SOURCES})
target_link_libraries (jk GL GLU glfw GLEW)

