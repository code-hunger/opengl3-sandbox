#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <vector>
#include <memory>

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
	printf("App initialization...\n");
	try {
		glfwSetErrorCallback(error_callback);

		if (!glfwInit()) {
			throw "Glfw init fail!";
		}
	} catch (const char *e) {
		printf("Exception handled from App constructor!!\n\n%s\n\n", e);
		throw - 1;
	}
	printf("App ctor exits successfully\n");
}

void App::boot()
{
	printf("Booting app...\n");

	try {
		glewExperimental = true;
		if (glewInit() != GLEW_OK) {
			throw "Glew init fail";
		}

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

		auto vertexArray = std::make_unique<VertexArray>(vertices, sizeof vertices, trapezoid,
		                          sizeof trapezoid);
        vertexArray->build();
		vertexArrays.push_back(std::move(vertexArray));
	} catch (const char *e) {
		printf("%s\n", e);
		throw - 1;
	}
	printf("App booted successfully!\n");
}

void App::run(Window &window)
{
    window.setKeyCallback(key_callback);
	printf("Start running!\n");
	double prevTime = glfwGetTime();
	while (!window.shouldClose()) {
		double time = glfwGetTime(), deltaTime = time - prevTime;
		prevTime = time;
		window.render(deltaTime, shaderPrograms, vertexArrays);
	}
	printf("Stop running!\n");
}

void App::createShaderPrograms()
{
	printf("Creating shader programms...\n");

	Shader frag_sh(readFile("fragment_shader.glsl"), GL_FRAGMENT_SHADER),
	    vert_sh(readFile("vertex_shader.glsl"), GL_VERTEX_SHADER);

	frag_sh.compile();
	vert_sh.compile();

	auto program = std::make_unique<ShaderProgram>(vert_sh, frag_sh);
	program->link();

	shaderPrograms.push_back(std::move(program));
    
	printf("Shader programs created!\n");
}

App::~App()
{
	printf("App destructor.\nGLFW terminate!\n");
	glfwTerminate();
}
