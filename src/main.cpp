#include <cstdio>

#include "Game.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Window.h"
#include "config.h"
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
		printf("Something bad happened. Error text:\n%s\n", e);
		return EXIT_FAILURE;
	}

	printf("Successfully exiting!!!\n");

	return EXIT_SUCCESS;
}
