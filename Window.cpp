#include "Window.h"

Window::Window()
{
	printf("Window initialization...\n");
	if (!hintsSet) {
		setHints();
	}

	if (!window) {
		throw "A window could not be created!";
	}

	trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 1.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	printf("Window ctor exits successfully\n");
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

	trans = glm::rotate(trans, (float)deltaTime, glm::vec3(1.0, 0.0, 0.0));

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::setKeyCallback(GLFWkeyfun callback)
{
	glfwSetKeyCallback(window, callback);
}

void Window::setHints()
{
	printf("Set hints!\n");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}
