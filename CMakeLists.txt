cmake_minimum_required(VERSION 2.8.9)
project (opengl-sandbox)

include_directories(glm/glm)

file(GLOB SOURCES "*.cpp")

add_executable(opengl-sandbox ${SOURCES})
target_link_libraries (opengl-sandbox GL GLU glfw GLEW)

