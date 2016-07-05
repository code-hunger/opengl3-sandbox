#include "Maze.h"

inline VertexArray generateVertArr(Lines lines)
{
	puts("Generating vertex array for the maze\n");
	float points[100 * 3];
	int i = 0;
	for (const auto &line : lines) {
		points[i++] = line.a.x;
		points[i++] = line.a.y;
		points[i++] = line.b.x;
		points[i++] = line.b.y;
	}

	VertexArray va(&(points[0]), i);
	va.build(2, false);
	return va;
}

Maze::Maze(Lines paths) : paths(paths), vertArray(generateVertArr(paths)) {}

void Maze::draw(GLenum mode) { vertArray.draw(mode); }

Maze::~Maze() {}
