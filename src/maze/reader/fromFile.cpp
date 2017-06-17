#include "graphics/include/config.h"
#include "math/include/types_io.h"
#include <fstream>
#include <string>

using namespace math;

static std::string getMazeFileName(unsigned maze_id)
{
	std::string fileName(MAZE_DIRECTORY "/maze");
	fileName += std::to_string(static_cast<unsigned>(maze_id));
	fileName += ".txt";
	return fileName;
}

static std::ifstream openMazeFile(unsigned maze_id)
{
	std::ifstream input{getMazeFileName(maze_id)};

	if (input.fail()) {
		throw "File does not exist!";
	}

	return input;
}

static WideRoads fetchLinesFromMaze(std::ifstream& input)
{
	WideRoad2 line{{}, {}};
	WideRoads lines;
	while (input >> line) {
		lines.push_back(line);
	}

	return lines;
}

WideRoads fetchLinesFromMaze(unsigned maze_id)
{
	std::ifstream input = openMazeFile(maze_id);
	return fetchLinesFromMaze(input);
}
