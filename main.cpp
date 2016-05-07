#include "App.h"
#include "Window.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	printf("Recieved key event! Key: %d, scancode : %d, action : %d, "
	       "mods: %d\n",
	       key, scancode, action, mods);
}

int main()
{
	try {
		App app;
        Window::setHints();
		Window window;
        window.setKeyCallback(key_callback);
		app.boot();
        app.run(window);
	} catch (int e) {
        printf("Something bad happened. Code %d\n", e);
		return e;
	}

	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // or GL_LINES or GL_POINT
    
    printf("Successfully exiting!!!\n");

	return EXIT_SUCCESS;
}
