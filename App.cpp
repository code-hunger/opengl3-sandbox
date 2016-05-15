#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <memory>
#include <vector>

#include "App.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "utils.h"

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
            +.5f, +.5f, -.5f, 0.f, 1.f, 0.f, // back    top right // 0
            -.5f, +.5f, -.5f, 1.f, 0.f, 0.f, // back    top left  // 1
            -.5f, -.5f, -.5f, .0f, .0f, 1.f, // back bottom left  // 2
            +.5f, -.5f, -.5f, 0.f, .5f, 0.f, // back bottom right // 3

            -.5f, +.5f,  .5f, .5f, 0.f, 0.f, // face    top left  // 4
            +.5f, +.5f,  .5f, 0.f, .0f, .9f, // face    top right // 5
            -.5f, -.5f, +.5f, 1.f, 1.f, 0.f, // face bottom left  // 6
            +.5f, -.5f, +.5f, 1.f, 1.f, 1.f, // face bottom right // 7
            /* -.5f,  -.5f, -.5f, .0f, .3f, .8f, */
            /* -.3f,   .5f, -.2f, .8f, .0f, .5f, */
            /*  .5f,   .5f,  .3f, 1.f, 1.f, 1.f, */
            /*  .5f,  -.5f,  .2f, 1.f, 1.f, 0.f, */
            /* -.75f,  .0f,  .1f, .0f, .5f, .5f */
        };

        GLuint trapezoid[] = {
            0, 1, 2, // back
            0, 2, 3, 
            2, 3, 6, // bottom
            3, 6, 7, 
            6, 4, 1, // left
            1, 6, 2,
            4, 5, 0, 5, // top
            5, 4,6, 7 // face
            /* 2, 1, 3, */

            /* 1, 2, 4, // left */
            /* 4, 6, 2, */
        };
		// clang-format on

		auto vertexArray = std::make_unique<VertexArray>(
		    vertices, sizeof vertices, trapezoid, sizeof trapezoid);

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

	glEnable(GL_DEPTH_TEST);

	int width, height;
	window.getSize(width, height);
	glViewport(0, 0, width, height);
	glClearColor(1, 1.f, 1, 0);

	int frames = 0;
	double last_time_check = glfwGetTime();
	while (!window.shouldClose()) {
		double time = glfwGetTime(), deltaTime = time - prevTime;
		prevTime = time;
		++frames;
		if (time - last_time_check >= 1) {
			printf("\t %f since last check", frames / (time - last_time_check));
			last_time_check = glfwGetTime();
			frames = 0;
			int x, y;
			window.getCursorPos(&x, &y);
			printf("Cursor: %dx%d\n", x, y);
		}
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
