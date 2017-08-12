file(GLOB SOURCES "src/*.cpp")

include_directories(.)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

if(WIN32) 
	set(GLFW_LIBRARIES "glfw3")
else() 
	set(GLFW_LIBRARIES "glfw")
endif()

add_library(graphics ${SOURCES})
target_link_libraries(graphics ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES}) 
