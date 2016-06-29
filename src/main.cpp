#include <cstdio>

#include "Window.h"

int main()
{
	try {
        Window::setHints();
		Window window;
        window.run();
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
