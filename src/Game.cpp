#include "Game.h"

#include "HomeScreen.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

#include <GL/glew.h>

Game::Game() : screen(std::make_unique<HomeScreen>())
{
	printf("Game constructor! HomeScreen constructed.\n");
}

void Game::render(double deltaTime, const State &state) const
{
	screen->render(deltaTime, state);
}

Game::~Game() { printf("Game class destructor!\n"); }
