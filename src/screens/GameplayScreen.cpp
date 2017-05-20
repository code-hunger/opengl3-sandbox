#include "GameplayScreen.h"

#include "graphics/config.h"
#include "graphics/utils.h"
#include "logger/logger.h"
#include "math/types_io.h"
#include "maze_builder/builder.h"

#include "Maze.h"
#include <GLFW/glfw3.h>

using namespace math;

std::string getMazeFileName(unsigned maze_id)
{
	std::string fileName(MAZE_DIRECTORY "/maze");
	fileName += std::to_string(static_cast<unsigned>(maze_id));
	fileName += ".txt";
	return fileName;
}

std::ifstream openMazeFile(unsigned maze_id)
{
	std::ifstream input(getMazeFileName(maze_id));

	if (input.fail()) {
		throw "File does not exist!";
	}

	return input;
}

WideRoads fetchLinesFromMaze(std::ifstream& input)
{
	WideRoad2 line{{}, {}};
	WideRoads lines;
	while (input >> line) {
		lines.push_back(line);
	}

	return lines;
}

Maze getMazeFromFile(ushort maze_id, bool join_it, ushort max_lines)
{
	std::ifstream input = openMazeFile(maze_id);
	WideRoads lines = fetchLinesFromMaze(input);

	const ColorSegmentList& walls =
	    Builder{join_it, max_lines}.build_from_paths(lines);
	return Maze::build(std::move(lines), walls);
}

GameplayScreen::GameplayScreen(ushort maze_id, bool join_it, ushort max_lines)
    : maze(getMazeFromFile(maze_id, join_it, max_lines))
{
	ships.addShip({Point2{17, 37}, 2});
}

void update(const double deltaTime, State& state, ShipsCollection& ships)
{
	if (state.keys[GLFW_KEY_ESCAPE]) {
		state.shouldClose = true;
	}
	ships.update(state, deltaTime);
}

void render(const double deltaTime, Maze& maze, const ShipsCollection& ships)
{
	(void)deltaTime;
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
