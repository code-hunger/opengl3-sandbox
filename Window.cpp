#include "Window.h"

Window::Window(GLFWwindow *window) : window(window)
{
	printf("Window constructed!\n");
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

void Window::render(const double deltaTime, const ShaderPrograms &programs,
                    const VertexArrays &vertArrays)
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

	float scale_factor =
	    static_cast<float>(2 + cos(glfwGetTime()) * 2) * 10 + 2;

	glm::mat4 model;
	model = glm::rotate(model, scale_factor, glm::vec3(0.8f, 1.0f, .0f));

	glm::mat4 view;
	view = glm::translate(
	    view, glm::vec3(-5 + deltaX * 2, -deltaY * 3, -scale_factor));

	glm::mat4 proj;
	proj = glm::perspective(45.f, 1.0f, 0.1f, 100.f);

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE,
	                   glm::value_ptr(proj * view * model));

	programs[0]->use();
	vertArrays[0]->draw(GL_TRIANGLES, 0);

	for (int i = 0; i < 3; ++i) {
		view = glm::translate(view, glm::vec3(0, 0, -10 * i));
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE,
		                   glm::value_ptr(proj * view * model));
		vertArrays[0]->draw(GL_TRIANGLES, 0);
	}

	view = glm::translate(view, glm::vec3(10, 0, 30));
	for (int i = 0; i < 3; ++i) {
		view = glm::translate(view, glm::vec3(0, 0, -10 * i));
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE,
		                   glm::value_ptr(proj * view * model));
		vertArrays[0]->draw(GL_TRIANGLES, 0);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::setKeyCallback(GLFWkeyfun callback)
{
	glfwSetKeyCallback(window, callback);
}

Window::~Window()
{
	printf("Window destructor. Destroy window!\n");
	glfwDestroyWindow(window);
}
