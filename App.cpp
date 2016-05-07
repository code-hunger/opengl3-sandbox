#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "Shader.h"
#include "ShaderProgram.h"
#include "utils.h"
#include "App.h"
#include <vector>

App::App() : shaderPrograms()
{
	try {
		init();
	} catch (const char *e) {
		printf("Exception handled from constructor!!\n\n%s\n\n", e);
	}
}
void App::run()
{
	while (!glfwWindowShouldClose(window)) {
		int width, height;
		/*  @TODO double time = glfwGetTime(); */
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0.f, 0, 0);

		trans = glm::rotate(trans, 0.05f, glm::vec3(1.0, 0.0, 0.0));

		glUniformMatrix4fv(transfLoc, 1, GL_FALSE, glm::value_ptr(trans));

		shaderProgram.use();

		myVertexArray.draw(GL_TRIANGLE_FAN, 0, sizeof trapezoid);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void App::init()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		throw "Glfw init fail!";
	}

	GLFWwindow *window = glfwCreateWindow(640, 480, "Title", NULL, NULL);

	if (!window) {
		throw "No window!";
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw "Glew init fail";
	}
}
void App::createShaderPrograms()
{
	Shader frag_sh(readFile("fragment_shader.glsl"), GL_FRAGMENT_SHADER),
	    vert_sh(readFile("vertex_shader.glsl"), GL_VERTEX_SHADER);

	frag_sh.compile();
	vert_sh.compile();

	ShaderProgram shaderProgram(vert_sh, frag_sh);
	shaderProgram.link();

	GLint transformLoc =
	    glGetUniformLocation(app.getShaderProgram().id, "transform");
}
void error_callback(int error, const char *desc)
{
	printf("Error callback! #%d: %s\n", error, desc);
}
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		change_color = !change_color;
