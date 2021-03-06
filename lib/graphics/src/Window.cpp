#include "Window.h"

#include "GlfwWrapper.h"
#include "State.h"
#include "logger/logger.h"

#include <GLFW/glfw3.h>

#define WIDTH 700
#define HEIGHT 700

GLFWwindow* createWindow(const char* title)
{
	return glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
}

void keyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action,
                 int mods)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	window->keyCallback(static_cast<ushort>(key), scancode, action, mods);
}

Dimentions getWindowSize(GLFWwindow* window)
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	return {static_cast<ushort>(w), static_cast<ushort>(h)};
}

Window::Window(std::shared_ptr<GlfwWrapper> glfwWrapper)
    : glfwWrapper{glfwWrapper},
      window(createWindow("First Window Title")), size{getWindowSize(window)}
{
	if (!window) throw "A window could not be created!";

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) throw "Glew init fail";
	if (!GLEW_VERSION_3_3) throw "No glew_version_3_3!";

	glfwSetWindowUserPointer(window, static_cast<void*>(this));
	glfwSetKeyCallback(window, ::keyCallback);

	LOG("Window created!");
}

void Window::run(void* renderObject, Renderer& renderFunction)
{
	glfwMakeContextCurrent(window);

	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	updateSize();

	glViewport(0, 0, size.width, size.height);
	glClearColor(0, 0, 0, 0);

	State state{0, 0, 0, keys};

#ifdef ANTI_ALIASING
	glEnable(GL_MULTISAMPLE);
#endif

	while (!state.shouldClose && !glfwWindowShouldClose(window)) {
		double time = glfwGetTime(), deltaTime = time - prevTime;
		prevTime = time;

		if (print_fps) LOG("%f FPS  ", 1.0 / deltaTime);

		glfwSwapBuffers(window);
		glfwPollEvents();

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		state.mouseX = static_cast<int>(mouseX);
		state.mouseY = static_cast<int>(mouseY);

		// renderer.render(deltaTime, state);
		renderFunction(renderObject, deltaTime, state);
	}
}

void Window::updateSize()
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	size.width = static_cast<ushort>(w);
	size.height = static_cast<ushort>(h);
}

void Window::keyCallback(ushort key, int scancode, int action, int mods) &
{
	if (GLFW_PRESS == action) {
		keys[key] = true;
	} else if (GLFW_RELEASE == action) {
		keys[key] = false;
	}
	return;

	LOG("Recieved key event! Key: %d, scancode : %d, action : %d, "
	    "mods: %d\n",
	    key, scancode, action, mods);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	LOG << "Window destroyed!";
}
