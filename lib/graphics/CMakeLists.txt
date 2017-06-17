file(GLOB SOURCES "src/*.cpp")

include_directories(.)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library(graphics ${SOURCES})
target_link_libraries(graphics ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES}) 
