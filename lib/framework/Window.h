#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
class GLFWwindow;
struct Renderer;

class Window
{
public:
	Window();
	virtual ~Window();

	void run(const Renderer&);

	static void prepareOpenGL();

	Window(const Window &) = delete;
	void operator=(const Window &) = delete;

protected:
    GLFWwindow *const window;

	static bool hintsSet;

    bool keys[1024];

	GLubyte width = 0, height = 0;
	void updateSize();

	bool shouldClose() const;

	void keyCallback(int key, int scancode, int action, int mods);
};

#endif /* end of include guard: WINDOW_H */
