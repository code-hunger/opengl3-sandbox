#include "Maze.h"
#include <cassert>
#include <cmath>

#define MAX_LINES 10
#define PI 3.141592653589793238462643383279502884L

Maze Maze::fromPaths(Ways paths)
{
	assert(paths.size() <= MAX_LINES);
	puts("Generating vertex array for the maze");

	float points[MAX_LINES * 4 * 3];

	Lines walls;

	int i = 0;

	for (const auto& way : paths) {
		const Segment2& line = way.line;
		double line_angle = atan((line.b.y - line.a.y) / (line.b.x - line.a.x));
		float angle_sin = static_cast<float>(sin(line_angle)),
		      angle_cos = static_cast<float>(cos(line_angle)),
		      deltaXA = way.width_a * angle_sin,
		      deltaXB = way.width_b * angle_sin,
		      deltaYA = way.width_a * angle_cos,
		      deltaYB = way.width_b * angle_cos;

		Segment2 upper{line.a.x + deltaXA, line.a.y - deltaYA,
		               line.b.x + deltaXB, line.b.y - deltaYB},
		    lower{line.b.x - deltaXB, line.b.y + deltaYB, line.a.x - deltaXA,
		          line.a.y + deltaYA};

		walls.insert(upper);
		points[i++] = upper.a.x;
		points[i++] = upper.a.y;

		points[i++] = upper.b.x;
		points[i++] = upper.b.y;

		points[i++] = upper.b.x;
		points[i++] = upper.b.y;

		points[i++] = lower.a.x;
		points[i++] = lower.a.y;

		points[i++] = upper.a.x;
		points[i++] = upper.a.y;

		points[i++] = lower.b.x;
		points[i++] = lower.b.y;

		walls.insert(lower);
		points[i++] = lower.a.x;
		points[i++] = lower.a.y;

		points[i++] = lower.b.x;
		points[i++] = lower.b.y;
	}

	VertexArray va(&(points[0]), i);
	va.build(2, false);

	return {paths, walls, va};
}

void Maze::draw(GLenum mode) { vertArray.draw(mode); }
