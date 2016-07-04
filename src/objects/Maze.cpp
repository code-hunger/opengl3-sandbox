#include "Maze.h"

inline VertexArray generateVertArr(Lines lines)
{
	puts("Generating vertex array for the maze\n");
	float points[10 * 2] = {0};
	int i = 0;
	for (const auto &line : lines) {
		points[i++] = line.a.x;
		points[i++] = line.a.y;
		points[i++] = line.b.x;
		points[i++] = line.b.y;
		printf("This line: %dx%d to %dx%d\n", line.a.x, line.a.y, line.b.x,
		       line.b.y);
	}

	printf("Count: %d\n", i);

	VertexArray va(&(points[0]), i);
	va.build(2, false);
	return va;
}

Maze::Maze(Lines paths) : paths(paths), vertArray(generateVertArr(paths)) {}

void Maze::draw() { vertArray.draw(GL_LINES); }

Maze::~Maze() {}
