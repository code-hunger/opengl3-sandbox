#include "Window.h"

Window::Window(GLFWwindow *window): window(window)
{
	printf("Window initialization...\n");
	trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 1.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	printf("Window ctor exits successfully\n");
}

void Window::getSize(int &width, int &height)
{
	glfwGetFramebufferSize(window, &width, &height);
}

void Window::render(const double deltaTime, const ShaderPrograms &programs,
                    const VertexArrays &vertArrays)
{
	int width, height;
	getSize(width, height);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0.f, 0, 0);

	static int transfLoc = glGetUniformLocation(programs[0]->id, "transform");
	glUniformMatrix4fv(transfLoc, 1, GL_FALSE, glm::value_ptr(trans));

	programs[0]->use();
	vertArrays[0]->draw(GL_TRIANGLE_FAN, 0, 7);

	trans = glm::rotate(trans, static_cast<float>(deltaTime), glm::vec3(1.0, 0.0, 0.0));

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
