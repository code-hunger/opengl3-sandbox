#include "Window.h"

#include <cstdlib>

using glm::vec3;
using glm::mat4;
using glm::vec4;
using glm::translate;

#define float(a) static_cast < float > (a)

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

	int width, height;
	getSize(width, height);
	glViewport(0, 0, width, height);
	glClearColor(1, 1.f, 1, 0);
}

void Window::getSize(int &width, int &height) const
{
	glfwGetFramebufferSize(window, &width, &height);
}

void Window::getCursorPos(int *x, int *y) const
{
	double dx, dy;
	glfwGetCursorPos(window, &dx, &dy);
	*x = static_cast<int>(dx);
	*y = static_cast<int>(dy);
}

void Window::update(double deltaTime)
{
	glfwSwapBuffers(window);
	glfwPollEvents();

	float cameraSpeed = float(deltaTime) * 20;
	mat4 transl;
	if (keys[GLFW_KEY_W] xor keys[GLFW_KEY_S]) {
		float isForward = 0;
		if (keys[GLFW_KEY_W]) {
			isForward = 1;
		} else if (keys[GLFW_KEY_S]) {
			isForward = -1;
		}
		transl = translate(transl, isForward * 4 * cameraSpeed *
		                               normalize(cameraFront));
	}

	if (keys[GLFW_KEY_D] xor keys[GLFW_KEY_A]) {
		float isRight = 0;
		if (keys[GLFW_KEY_D]) {
			isRight = 1;
		} else if (keys[GLFW_KEY_A]) {
			isRight = -1;
		}
		transl =
		    translate(transl, isRight * cameraSpeed *
		                          normalize(glm::cross(cameraFront, cameraUp)));
	}

	if (keys[GLFW_KEY_J] xor keys[GLFW_KEY_K]) {
		float isUp = 0;
		if (keys[GLFW_KEY_K]) {
			isUp = 1;
		} else if (keys[GLFW_KEY_J]) {
			isUp = -1;
		}
		transl = translate(transl, isUp * cameraSpeed * normalize(cameraUp));
	}

	cameraPos = transl * vec4(cameraPos.x, cameraPos.y, cameraPos.z, 1);

	double _x, _y;
	glfwGetCursorPos(window, &_x, &_y);
	int x = static_cast<int>(_x), y = static_cast<int>(_y);
	int deltaX = x - lastMouseX, deltaY = -y + lastMouseY;

	lastMouseX = x;
	lastMouseY = y;

	GLfloat sensitivity = 3 * float(deltaTime);
	yaw += sensitivity * float(deltaX);
	pitch += sensitivity * float(deltaY);

	float _yaw = glm::radians(yaw), _pitch = glm::radians(pitch);

	glm::vec3 front;
	front.x = float(cos(_yaw) * cos(_pitch));
	front.y = float(sin(_pitch));
	front.z = float(sin(_yaw) * cos(_pitch));
	cameraFront = glm::normalize(front);
}

void Window::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}

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

void Window::render(const double deltaTime, const ShaderPrograms &programs,
                    VertexArrays const &vertArrays) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	(void)deltaTime;
	int mvpLoc = glGetUniformLocation(programs[0]->id, "model_view_projection");

	int mouseX, mouseY;
	this->getCursorPos(&mouseX, &mouseY);
	int width, height;
	this->getSize(width, height);

	mat4 proj, model,
	    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	model = glm::rotate(model, float(glfwGetTime() * 8), vec3(0.8f, 1.0f, .0f));
	proj = glm::perspective(45.f, 1.0f, 0.1f, 250.f);

	programs[0]->use();

	srand(42);

	int MAX_CUBE = 100;

	for (int i = 0; i < MAX_CUBE; ++i) {
		mat4 _view =
		    glm::translate(view, vec3(rand() % MAX_CUBE, rand() % MAX_CUBE,
		                              rand() % MAX_CUBE));
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE,
		                   glm::value_ptr(proj * _view * model));
		vertArrays[0]->draw(GL_TRIANGLES, 0);
	}

	glUniformMatrix4fv(
	    mvpLoc, 1, GL_FALSE,
	    glm::value_ptr(proj * view * glm::mat4())); // don't rotate ground!
	vertArrays[2]->draw(GL_TRIANGLES, 0);
}

Window::~Window()
{
	printf("Window destructor. Destroy window!\n");
	glfwDestroyWindow(window);
}
