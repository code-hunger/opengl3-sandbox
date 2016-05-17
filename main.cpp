#include "App.h"
#include "Window.h"

#define WIN_WIDTH 766
#define WIN_HEIGHT 766

void error_callback(int error, const char *desc)
{
	printf("Error callback! #%d: %s\n", error, desc);
}

void start()
{
	if (!glfwInit()) {
		throw "Glfw init fail!";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window =
	    glfwCreateWindow(WIN_HEIGHT, WIN_WIDTH, "Title", NULL, NULL);
	if (!window) {
		throw "A window could not be created!";
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		throw "Glew init fail";
	}

	App app;
	app.run(Window(window));
}

int main()
{
	try {
		start();
	} catch (int e) {
		printf("Something bad happened. Code %d\n", e);
		return e;
	} catch (const char *e) {
		printf("Something bad happened. Text:\n%s\n", e);
		return EXIT_FAILURE;
	}

	printf("Successfully exiting!!!\n");

	return EXIT_SUCCESS;
}
