#include "Maze.h"
#include <cassert>
#include <cmath>

#define MAX_LINES 10
#define PI 3.141592653589793

Maze Maze::fromPaths(Lines lines)
{
	assert(lines.size() <= MAX_LINES);
	puts("Generating vertex array for the maze\n");

	float points[MAX_LINES * 4 * 3];

	int i = 0;
	for (const auto &line : lines) {
		double line_angle = atan((line.b.y - line.a.y) / (line.b.x - line.a.x));
		float angle_sin = static_cast<float>(sin(line_angle)),
		      angle_cos = static_cast<float>(cos(line_angle)),
		      deltaXA = line.weight_a * angle_sin,
		      deltaXB = line.weight_b * angle_sin,
		      deltaYA = line.weight_a * angle_cos,
		      deltaYB = line.weight_b * angle_cos;

		Line2 upper{line.a.x + deltaXA, line.a.y - deltaYA, line.b.x + deltaXB,
		            line.b.y - deltaYB},
		    lower{line.b.x - deltaXB, line.b.y + deltaYB, line.a.x - deltaXA,
		          line.a.y + deltaYA};

		points[i++] = upper.a.x;
		points[i++] = upper.a.y;

		points[i++] = upper.b.x;
		points[i++] = upper.b.y;

		points[i++] = lower.a.x;
		points[i++] = lower.a.y;

		points[i++] = lower.b.x;
		points[i++] = lower.b.y;

		/* points[i++] = line.a.x; */
		/* points[i++] = line.a.y; */

		/* points[i++] = line.b.x; */
		/* points[i++] = line.b.y; */
	}
    Lines walls;

	VertexArray va(&(points[0]), i);
	va.build(2, false);

	return {lines, walls, va};
}

void Maze::draw(GLenum mode) { vertArray.draw(mode); }

