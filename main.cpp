#include "App.h"
#include "Window.h"

int main()
{
    App app;
    Window window;
    app.boot();
    /* app.run(); */

	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // or GL_LINES or GL_POINT

	return EXIT_SUCCESS;
}
