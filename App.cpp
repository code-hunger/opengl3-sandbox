#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <vector>
#include <memory>

#include "Shader.h"
#include "ShaderProgram.h"
#include "utils.h"
#include "App.h"

bool paused = false;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		paused = !paused;
		printf("Pause!\n");
	}

	printf("Recieved key event! Key: %d, scancode : %d, action : %d, "
	       "mods: %d\n",
	       key, scancode, action, mods);
}

App::App()
{
	printf("App ctor.\n");

	try {
		createShaderPrograms();

		// clang-format off
        GLfloat vertices[] = {
            -.5f,  -.5f, -.5f, .0f, .3f, .8f,
            -.3f,   .5f, -.2f, .8f, .0f, .5f,
             .5f,   .5f,  .3f, 1.f, 1.f, 1.f,
             .5f,  -.5f,  .2f, 1.f, 1.f, 0.f,
            -.75f,  .0f,  .1f, .0f, .5f, .5f
        };
		// clang-format on

		GLuint trapezoid[] = {0, 1, 3, 4, 1, 2};

		auto vertexArray = std::make_unique<VertexArray>(vertices, sizeof vertices, trapezoid,
		                          sizeof trapezoid);

        vertexArray->build(3);
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
		window.pause = paused;
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
