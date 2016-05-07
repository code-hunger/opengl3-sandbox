#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <vector>

#include "Shader.h"
#include "ShaderProgram.h"
#include "utils.h"
#include "App.h"

void error_callback(int error, const char *desc)
{
	printf("Error callback! #%d: %s\n", error, desc);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	printf("Recieved key event! Key: %d, scancode : %d, action : %d, "
	       "mods: %d\n",
	       key, scancode, action, mods);
}

App::App()
{
    printf("App initialized!!\n");
	try {
		init();
		createShaderPrograms();

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
		this->vertexArrays.push_back(myVertexArray);
		myVertexArray.build();
	} catch (const char *e) {
		printf("Exception handled from constructor!!\n\n%s\n\n", e);
	}
}

void App::run()
{
	double prevTime = glfwGetTime();
	while (window.shouldClose()) {
		double time = glfwGetTime(), deltaTime = time - prevTime;
		prevTime = time;
		window.render(deltaTime, shaderPrograms, vertexArrays);
	}
}

void App::init()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		throw "Glfw init fail!";
	}

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

	shaderPrograms.push_back(shaderProgram);
}
