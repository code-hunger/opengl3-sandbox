#include "GameplayScreen.h"

#include "../maze/reader/fromFile.h"
#include "graphics/utils.h"
#include "logger/logger.h"
#include "maze_builder/builder.h"

#include "Maze.h"
#include <GLFW/glfw3.h>

Maze getMazeFromFile(ushort maze_id, bool join_it, ushort max_lines)
{
	auto lines = fetchLinesFromMaze(maze_id);

	const auto& walls = Builder{join_it, max_lines}.build_from_paths(lines);
	return Maze::build(std::move(lines), walls);
}

GameplayScreen::GameplayScreen(ushort maze_id, bool join_it, ushort max_lines)
    : maze(getMazeFromFile(maze_id, join_it, max_lines))
{
	ships.addShip(math::Point2{17, 37});
	ships.addShip(math::Point2{40, 15});
}

void update(const double deltaTime, State& state, ShipsCollection& ships)
{
	if (state.keys[GLFW_KEY_ESCAPE]) {
		state.shouldClose = true;
	}
	ships.update(state, deltaTime);
}

void render(double, Maze& maze, ShipsCollection& ships)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	maze.draw(GL_LINES);
	ships.draw();
}

void GameplayScreen::work(const double deltaTime, State& state)
{
	update(deltaTime, state, ships);
	render(deltaTime, maze, ships);
}

GameplayScreen::~GameplayScreen() {}
