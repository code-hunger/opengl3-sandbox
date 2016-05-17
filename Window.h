#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#pragma GCC diagnostic pop

#include <memory>
#include <vector>

#include "ShaderProgram.h"
#include "VertexArray.h"

typedef std::vector<std::unique_ptr<ShaderProgram>> ShaderPrograms;
typedef std::vector<std::unique_ptr<VertexArray>> VertexArrays;

class Window
{
public:
	Window(GLFWwindow *window);
	virtual ~Window();

	bool shouldClose() const { return glfwWindowShouldClose(window); }

	void getSize(int &width, int &height) const;

	void getCursorPos(int *x, int *y) const;

	void use(); // use this method when using multiple windows. Call it before
	            // using a window

	void render(double deltaTime, const ShaderPrograms &,
	            const VertexArrays &) const;

	void keyCallback(int key, int scancode, int action, int mods);

	void static setHints();

	Window(const Window &) = delete;
	void operator=(const Window &) = delete;

private:
	GLFWwindow *const window;
	static const bool hintsSet{false};

	GLint transfLoc = 0;
};

#endif /* end of include guard: WINDOW_H */
