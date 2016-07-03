#include <cstdio>

#include "Game.h"
#include "Window.h"
#include <GL/glew.h>

int main()
{
	try {
		Window::prepareOpenGL();
		Window window;

		Game game;
		window.run(game);
	} catch (int e) {
		printf("Something bad happened. Code %d\n", e);
		return e;
	} catch (const char *e) {
		printf("Something bad happened. Error text: %s\n", e);
		return EXIT_FAILURE;
	}

	printf("Successfully exiting!!!\n");

	return EXIT_SUCCESS;
}
