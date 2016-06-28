#include <cstdio>

#include "Window.h"

void error_callback(int error, const char *desc)
{
	printf("Error callback! #%d: %s\n", error, desc);
}

int main()
{
	try {
        Window::setHints();
		Window window;
        window.run();

        /* App app; */
		/* app.run(window); */
	} catch (int e) {
		printf("Something bad happened. Code %d\n", e);
		return e;
	} catch (const char *e) {
		printf("Something bad happened. Error text:\n%s\n", e);
		return EXIT_FAILURE;
	}

	printf("Successfully exiting!!!\n");

	return EXIT_SUCCESS;
}
