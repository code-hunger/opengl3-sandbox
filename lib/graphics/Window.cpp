#include "Window.h"

#include "Renderer.h"
#include "State.h"
#include "config.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <cstdio>
#include <cstdlib>

#define WIDTH 700
#define HEIGHT 700
/* #define PRINT_FPS */

bool Window::hintsSet = false;

void Window::prepareOpenGL()
{
	if (!glfwInit()) {
		throw "Glfw init fail!";
	}

	glfwSetErrorCallback([](int code, const char* desc) {
		fprintf(stderr, "GLFW error #%d: %s\n", code, desc);
		throw "Error callback called!";
	});

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef ANTI_ALIASING
	glfwWindowHint(GLFW_SAMPLES, 4);
#endif

	Window::hintsSet = true;
}

inline GLFWwindow* createWindow(const char* title)
{
	return glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
}

Window::Window() : window(createWindow("First Window Title"))
{
	if (!window) {
		throw "A window could not be created!";
	}

	// Window::prepareOpenGL must be called before constructing a Window,
	// otherwise OpenGL doesn't work

	if (!hintsSet)
		throw "You must call Window::setHints before constructing window, "
		      "otherwise opengl won't work!";

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		throw "Glew init fail";
	}

	if (!GLEW_VERSION_3_3) {
		throw "No glew_version_3_3!";
	}

	glfwSetWindowUserPointer(window, static_cast<void*>(this));

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode,
	                              int action, int mods) {
		static_cast<Window*>(glfwGetWindowUserPointer(window))
		    ->keyCallback(key, scancode, action, mods);
	});
}

void Window::run(const Renderer& renderer)
{
	glfwMakeContextCurrent(window);

	int frames = 0;
	double prevTime = glfwGetTime()
#ifdef PRINT_FPS
	    ,
	       last_time_check = prevTime
#endif
	    ;

	glEnable(GL_DEPTH_TEST);

	updateSize();

	glViewport(0, 0, width, height);
	glClearColor(0, 0, 0, 0);

	State state{0, 0, 0, &(keys[0])};

#ifdef ANTI_ALIASING
	glEnable(GL_MULTISAMPLE);
#endif

	while (!state.shouldClose && !glfwWindowShouldClose(window)) {
		double time = glfwGetTime(), deltaTime = time - prevTime;
		prevTime = time;

		++frames;
#ifdef PRINT_FPS
		if (time - last_time_check >= 1) {
			printf("%f since last check\n", frames / (time - last_time_check));
			last_time_check = glfwGetTime();
			frames = 0;
		}
#endif
		glfwSwapBuffers(window);
		glfwPollEvents();

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		state.mouseX = static_cast<int>(mouseX);
		state.mouseY = static_cast<int>(mouseY);

		renderer.render(deltaTime, state);
	}
}

void Window::updateSize()
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	width = static_cast<GLushort>(w);
	height = static_cast<GLushort>(h);
}

void Window::keyCallback(int key, int scancode, int action, int mods)
{
	if (GLFW_PRESS == action) {
		keys[key] = true;
	} else if (GLFW_RELEASE == action) {
		keys[key] = false;
	}
	return;

	printf("Recieved key event! Key: %d, scancode : %d, action : %d, "
	       "mods: %d\n",
	       key, scancode, action, mods);
}

Window::~Window() { glfwDestroyWindow(window); }