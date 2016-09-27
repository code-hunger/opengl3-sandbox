file(GLOB SOURCES "*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/lib/include/graphics)

add_library(graphics ${SOURCES})
target_link_libraries(graphics GL GLU glfw GLEW)
