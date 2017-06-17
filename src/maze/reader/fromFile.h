#include <string>
#include <fstream>
#include "math/include/types_io.h"
#include "graphics/include/config.h"

static std::string getMazeFileName(unsigned maze_id)
{
	std::string fileName(MAZE_DIRECTORY "/maze");
	fileName += std::to_string(static_cast<unsigned>(maze_id));
	fileName += ".txt";
	return fileName;
}

std::ifstream openMazeFile(unsigned maze_id)
{
	std::ifstream input{getMazeFileName(maze_id)};

	if (input.fail()) {
		throw "File does not exist!";
	}

	return input;
}

math::WideRoads fetchLinesFromMaze(std::ifstream& input)
{
	math::WideRoad2 line{{}, {}};
	math::WideRoads lines;
	while (input >> line) {
		lines.push_back(line);
	}

	return lines;
}

math::WideRoads inline fetchLinesFromMaze(unsigned maze_id)
{
	std::ifstream input = openMazeFile(maze_id);
	return fetchLinesFromMaze(input);
}

