#include "App.h"
#include "Window.h"


int main()
{
	try {
        // order of execution is important!
		App app;
		Window::setHints();
		Window window;
		app.boot();
		app.run(window);
	} catch (int e) {
		printf("Something bad happened. Code %d\n", e);
		return e;
	}

	printf("Successfully exiting!!!\n");

	return EXIT_SUCCESS;
}
