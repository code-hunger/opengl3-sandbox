#include "Window.h"

using glm::vec3;
using glm::mat4;

Window::Window(GLFWwindow *window) : window(window)
{
	printf("Window constructed!\n");

	glfwSetWindowUserPointer(window, static_cast<void *>(this));
	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
	                              int action, int mods) {
		static_cast<Window *>(glfwGetWindowUserPointer(window))
		    ->keyCallback(key, scancode, action, mods);
	});
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::getSize(int &width, int &height)
{
	glfwGetFramebufferSize(window, &width, &height);
}

void Window::getCursorPos(int *x, int *y)
{
	double dx, dy;
	glfwGetCursorPos(window, &dx, &dy);
	*x = static_cast<int>(dx);
	*y = static_cast<int>(dy);
}

void Window::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		printf("Pause!\n");
	}

	printf("Recieved key event! Key: %d, scancode : %d, action : %d, "
	       "mods: %d\n",
	       key, scancode, action, mods);
}

void Window::render(const double deltaTime, const ShaderPrograms &programs,
                    VertexArrays const &vertArrays)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	(void)deltaTime;
	int mvpLoc = glGetUniformLocation(programs[0]->id, "model_view_projection");

	int mouseX, mouseY;
	this->getCursorPos(&mouseX, &mouseY);
	int width, height;
	this->getSize(width, height);

	float deltaX = static_cast<float>(width / 2 - mouseX) /
	               static_cast<float>(width),
	      deltaY = static_cast<float>(height / 2 - mouseY) /
	               static_cast<float>(height);
	(void)deltaX;
	(void)deltaY;

	int BOX_COUNT = 10, SPACE_BETWEEN = 10;

	float scale_factor =
	    static_cast<float>(2 + cos(glfwGetTime()) * 2) * 10 + 2;
	scale_factor = static_cast<float>(glfwGetTime() * 30);

	mat4 proj, model,
	    view = glm::lookAt(vec3(0, 2, 5), vec3(0, 0, 0), vec3(0, 1, 0));

	model = glm::rotate(model, scale_factor, vec3(0.8f, 1.0f, .0f));
	proj = glm::perspective(45.f, 1.0f, 0.1f, 100.f);

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE,
	                   glm::value_ptr(proj * view * model));

	programs[0]->use();

	vertArrays[0]->draw(GL_TRIANGLES, 0);

	for (int i = 0; i < BOX_COUNT; ++i) {
		view = glm::translate(view, vec3(0, 0, -SPACE_BETWEEN));
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE,
		                   glm::value_ptr(proj * view * model));
		vertArrays[0]->draw(GL_TRIANGLES, 0);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

Window::~Window()
{
	printf("Window destructor. Destroy window!\n");
	glfwDestroyWindow(window);
}
