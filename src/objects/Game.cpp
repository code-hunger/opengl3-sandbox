#include "Game.h"

#include "HomeScreen.h"
#include "graphics/include/Shader.h"
#include "graphics/include/ShaderProgram.h"
#include "graphics/include/VertexArray.h"
#include "tclap/CmdLine.h"

#include <GL/glew.h>

Game Game::fromCmd(int argc, char** argv)
{
	TCLAP::CmdLine cmd("Game title", ' ', "0.1");
	TCLAP::UnlabeledValueArg<ushort> maze_id(
	    "maze_id", "Choose the number of the maze to be loaded", false, 1,
	    "Id of maze to load", cmd);
	TCLAP::ValueArg<ushort> max_lines("m", "max_lines", "Max ways to be added",
	                                  false, 0, "Max ways to be added", cmd);
	TCLAP::SwitchArg dont_join_lines("n", "nojoin",
	                                 "Does not join intersecting lines", cmd);
	cmd.parse(argc, argv);

	return {maze_id.getValue(), dont_join_lines.getValue(),
	        max_lines.getValue()};
}

Game::Game(ushort maze_id, bool dont_join_lines, ushort max_lines)
    : screen(std::make_unique<HomeScreen>(maze_id, !dont_join_lines, max_lines))
{
}

void Game::render(const double deltaTime, State& state) const
{
	screen->render(deltaTime, state);
	screen->update(deltaTime, state);
}
