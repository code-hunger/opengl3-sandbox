#ifndef WINDOW_H
#define WINDOW_H

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ShaderProgram.h"
#include "VertexArray.h"

class GLFWwindow;

typedef std::vector<ShaderProgram> ShaderPrograms;
typedef std::vector<VertexArray> VertexArrays;

class Window
{
public:
	Window();
	virtual ~Window();

	void use(); // use this method when using multiple windows. Call it before
	            // using a window

	void update(double deltaTime);

	void run();

	static void setHints();

	Window(const Window &) = delete;
	void operator=(const Window &) = delete;

private:
	GLFWwindow *const window;
	static bool hintsSet;

	ShaderPrograms shaderPrograms{};
	VertexArrays vertexArrays{};

	int lastMouseX = 0, lastMouseY = 0;
	glm::vec3 cameraPos{glm::vec3(0, 2, 3)}, cameraFront{glm::vec3(0, 0, -1)},
	    cameraUp{glm::vec3(0, 1, 0)};

	bool keys[1024]{false};

	void createShaderPrograms();
	void getCursorPos(int *x, int *y) const;
	bool shouldClose() const;
	void getSize(int *width, int *height) const;
	void render(double deltaTime, const ShaderPrograms &,
	            const VertexArrays &) const;

	void keyCallback(int key, int scancode, int action, int mods);

	float yaw = 0, pitch = 0;
};

#endif /* end of include guard: WINDOW_H */
