file(GLOB SOURCES "*.cpp")

include_directories(include)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library(graphics ${SOURCES})
target_link_libraries(graphics ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES}) 
