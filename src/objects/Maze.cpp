#include "Maze.h"
#include <cassert>
#include <cmath>

#define MAX_LINES 10
#define PI 3.141592653589793

Maze Maze::fromPaths(Ways paths)
{
	assert(paths.size() <= MAX_LINES);
	puts("Generating vertex array for the maze");

	float points[MAX_LINES * 4 * 3];

	Lines walls;

	int i = 0;

	for (const auto &way : paths) {
        const Line2& line = way.line;
		double line_angle = atan((line.b.y - line.a.y) / (line.b.x - line.a.x));
		float angle_sin = static_cast<float>(sin(line_angle)),
		      angle_cos = static_cast<float>(cos(line_angle)),
		      deltaXA = way.width_a * angle_sin,
		      deltaXB = way.width_b * angle_sin,
		      deltaYA = way.width_a * angle_cos,
		      deltaYB = way.width_b * angle_cos;

		Line2 upper{line.a.x + deltaXA, line.a.y - deltaYA, line.b.x + deltaXB,
		            line.b.y - deltaYB},
		    lower{line.b.x - deltaXB, line.b.y + deltaYB, line.a.x - deltaXA,
		          line.a.y + deltaYA};

		bool noUpper = false, noLower = false;

		for (const auto &other : walls) {
			Point2 crossPoint;

			if (other.intersectsWith(lower, &crossPoint)) {
				noLower = true;
			}
			if (other.intersectsWith(upper, &crossPoint)) {
				noUpper = true;
			}
		}

		if (!noUpper) {
			walls.insert(Lines::value_type(upper));
			points[i++] = upper.a.x;
			points[i++] = upper.a.y;

			points[i++] = upper.b.x;
			points[i++] = upper.b.y;
		}

		if (!noLower) {
			walls.insert(Lines::value_type(lower));
			points[i++] = lower.a.x;
			points[i++] = lower.a.y;

			points[i++] = lower.b.x;
			points[i++] = lower.b.y;
		}
	}

	VertexArray va(&(points[0]), i);
	va.build(2, false);

	return {paths, walls, va};
}

void Maze::draw(GLenum mode) { vertArray.draw(mode); }
