#include "HomeScreen.h"

#include "graphics/include/config.h"
#include "graphics/include/utils.h"
#include "logger/include/logger.h"
#include "math/include/types_io.h"
#include "maze_builder/include/builder.h"

#include "Maze.h"

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

HomeScreen::HomeScreen(ushort maze_id, bool join_it, ushort max_lines)
    : screen_elements{}
{
	screen_elements.emplace_back(
	    std::make_unique<Maze>(getMazeFromFile(maze_id, join_it, max_lines)));
}

void update(const double deltaTime, State& state)
{
	(void)deltaTime;
	if (state.keys[256]) {
		state.shouldClose = true;
	}
}

void render(const double deltaTime, Drawable& maze)
{
	(void)deltaTime;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	maze.draw(GL_LINES);
}

void HomeScreen::work(const double deltaTime, State& state)
{
	update(deltaTime, state);
	for (auto& element : screen_elements) {
		render(deltaTime, *element);
	}
}

HomeScreen::~HomeScreen() {}
