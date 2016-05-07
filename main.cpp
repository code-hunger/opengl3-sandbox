#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "utils.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Crashable.h"
#include "CrashHandler.h"
#include "App.cpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wpedantic"
#include "glm/glm/fwd.hpp"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#pragma GCC diagnostic pop

int main()
{
    App app;


	glm::mat4 trans;
	trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 1.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	// clang-format off
	GLfloat vertices[] = {
	    -0.5f,  -0.5f, .0f, .3f, .8f,
	    -0.3f,   0.5f, .8f, 0.f, .5f,
	     0.5f,   0.5f, 1.f, 1.f, 1.f,
	     0.5f,  -0.5f, 1.f, 1.f, 0.f,
	    -0.75f,  0.f,  .0f, .5f, .5f
	};
	// clang-format on

	GLuint trapezoid[] = {0, 1, 3, 4, 1, 2};

	VertexArray myVertexArray(vertices, sizeof vertices, trapezoid,
	                          sizeof trapezoid);
	myVertexArray.build();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // or GL_LINES or GL_POINT


	glfwTerminate();

	printf("Exit!\n");

	return EXIT_SUCCESS;
}
