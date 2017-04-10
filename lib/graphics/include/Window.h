#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <iostream>
#include <memory>
#include <vector>
class GLFWwindow;
struct State;

// using Renderer = std::function<void(double deltaTime, State&)>;
using Renderer = void (&)(void*, double deltaTime, State&);

class Window
{
	Window();

	friend class GlfwWrapper;
	friend void keyCallback(GLFWwindow*, int, int, int, int);

public:
	~Window();

	// is const& needed?
	void run(void*, Renderer);

	template <typename T> inline void run(T& obj, Renderer render)
	{
		run(static_cast<void*>(&obj), render);
	}

	Window(const Window&) = delete;
	Window& operator=(Window) = delete;

	Window(Window&&) = default;

protected:
	GLFWwindow* window;

	GLushort width, height;

	bool keys[1024]{false};

	void updateSize();

	bool shouldClose() const;

	void keyCallback(ushort key, int scancode, int action, int mods) &;
};

#endif /* end of include guard: WINDOW_H */
