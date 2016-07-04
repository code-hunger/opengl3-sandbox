#include "Game.h"

#include "HomeScreen.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

#include <GL/glew.h>

Game::Game() : screen(std::make_unique<HomeScreen>())
{
	printf("Game constructor - home screen constructed.\n");
}

void Game::render(const double deltaTime, State &state) const
{
	screen->render(deltaTime, state);
	screen->update(deltaTime, state);
}

Game::~Game() { printf("Game class destructor!\n"); }
