#include "App.h"
#include "Window.h"

int main()
{
	try {
		App app;
		Window window;
		app.boot();
	} catch (int e) {
        printf("Something bad happened. Code %d\n", e);
		return e;
	}
	/* app.run(); */

	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // or GL_LINES or GL_POINT

	return EXIT_SUCCESS;
}
