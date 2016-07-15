#include "Game.h"

#include "HomeScreen.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

#include <GL/glew.h>

Game::Game(unsigned short maze_id)
    : screen(std::make_unique<HomeScreen>(maze_id))
{
}

void Game::render(const double deltaTime, State& state) const
{
	screen->render(deltaTime, state);
	screen->update(deltaTime, state);
}

Game::~Game() {}
