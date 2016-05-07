#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window ();
    virtual ~Window ();

private:
    GLFWwindow *instance;
    static const bool hintsSet;
    static void setHints();
};

#endif /* end of include guard: WINDOW_H */
