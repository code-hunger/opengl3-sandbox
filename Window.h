#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wpedantic"
#include "glm/glm/fwd.hpp"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#pragma GCC diagnostic pop

#include <vector>

#include "ShaderProgram.h"
#include "VertexArray.h"

typedef std::vector<ShaderProgram> ShaderPrograms;
typedef std::vector<VertexArray> VertexArrays;

class Window
{
public:
	Window();
	virtual ~Window()
	{
		printf("Window destructor. Destroy window!\n");
		glfwDestroyWindow(window);
	}

	bool shouldClose() { return glfwWindowShouldClose(window); }

	void getSize(int &width, int &height)
	{
		glfwGetFramebufferSize(window, &width, &height);
	}

	void use(); // use this method when using multiple windows. Call it before
	            // using a window

	void setKeyCallback(GLFWkeyfun callback);

	void render(double deltaTime, ShaderPrograms &, VertexArrays &);

	void static setHints();

private:
	Window(const Window &);
	void operator=(const Window &);

	GLFWwindow *window{glfwCreateWindow(640, 480, "Title", NULL, NULL)};
	static const bool hintsSet{false};

	glm::mat4x4 trans{};
	GLint transfLoc = 0;
};

#endif /* end of include guard: WINDOW_H */
