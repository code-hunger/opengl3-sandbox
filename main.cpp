#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "utils.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Crashable.h"
#include "CrashHandler.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wpedantic"
#include "glm/glm/fwd.hpp"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#pragma GCC diagnostic pop

bool change_color = true;

void error_callback(int error, const char *desc)
{
	printf("Error #%d: %s\n", error, desc);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		change_color = !change_color;

	printf("Recieved key event! Key: %d, scancode: %d, action: %d, "
	       "mods: %d\n",
	       key, scancode, action, mods);
}

int main()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Title", NULL, NULL);

	CrashHandler crash(glfwTerminate);

	crash.handle(window, "Window not created");

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	crash.handle((void *)(glewInit() == GLEW_OK),
	             "Glew initialization failed!\n");

	Shader frag_sh(readFile("fragment_shader.glsl"), GL_FRAGMENT_SHADER),
	    vert_sh(readFile("vertex_shader.glsl"), GL_VERTEX_SHADER);

	crash.handle(frag_sh, frag_sh.compile());
	crash.handle(vert_sh, vert_sh.compile());

	ShaderProgram shaderProgram(vert_sh, frag_sh);
	crash.handle(shaderProgram, shaderProgram.link());

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

	glfwSetKeyCallback(window, key_callback);

	GLint transformLoc = glGetUniformLocation(shaderProgram.id, "transform");

	while (!glfwWindowShouldClose(window)) {
		int width, height;
		/*  @TODO double time = glfwGetTime(); */
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0.f, 0, 0);

		trans = glm::rotate(trans, 0.05f, glm::vec3(1.0, 1.0, 0.0));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		shaderProgram.use();

		myVertexArray.draw(GL_TRIANGLE_FAN, 0, sizeof trapezoid);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	printf("Exit!\n");

	return EXIT_SUCCESS;
}
