#include "Game.h"

#include "HomeScreen.h"
#include "graphics/include/Shader.h"
#include "graphics/include/ShaderProgram.h"
#include "graphics/include/VertexArray.h"

#include <GL/glew.h>

Game::Game(ushort maze_id, bool dont_join_lines, ushort max_lines)
    : screen(std::make_unique<HomeScreen>(maze_id, !dont_join_lines, max_lines))
{
}

void Game::render(const double deltaTime, State& state) const
{
	screen->render(deltaTime, state);
	screen->update(deltaTime, state);
}
