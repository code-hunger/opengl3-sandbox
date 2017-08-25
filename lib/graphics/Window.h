#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>

class GLFWwindow;
struct State;

// using Renderer = std::function<void(double deltaTime, State&)>;
using Renderer = void (&)(void*, double deltaTime, State&);

struct Dimentions
{
	unsigned short width, height;
};

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

	void setPrintFps(bool print_fps) { this->print_fps = print_fps; }

protected:
	GLFWwindow* window;

	Dimentions size;

	bool keys[1024]{false}, print_fps = false;

	void updateSize();

	bool shouldClose() const;

	void keyCallback(unsigned short key, int scancode, int action, int mods) &;
};

#endif /* end of include guard: WINDOW_H */
