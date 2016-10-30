#include "Game.h"
#include "graphics/include/Window.h"
#include <GL/glew.h>

#include <cstdio>

#include <iostream>

int main(int argc, char** argv)
{
	try {
		Window::prepareOpenGL();
		Window window;

		// a Window is needed to construct a game
		Game game = Game::fromCmd(argc, argv);

		window.run(game);
	} catch (int e) {
		printf("Something bad happened. Code %d\n", e);
		return e;
	} catch (const char* e) {
		printf("Something bad happened. Error text: %s\n", e);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
