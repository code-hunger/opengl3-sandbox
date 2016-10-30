file(GLOB SOURCES "*.cpp")

include_directories(include)

add_library(graphics ${SOURCES})
target_link_libraries(graphics GL GLU glfw GLEW)
