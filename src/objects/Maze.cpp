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

	Lines wallsP, wallsS; // primary and secondary

	struct CrossRoad
	{
		std::vector<Segment2*> lines;
	};

	std::vector<CrossRoad> cross_roads;

	for (const auto& way : paths) {
		const Segment2 line = way.getSegmnet2();
		double line_angle = atan((line.b.y - line.a.y) / (line.b.x - line.a.x));
		float angle_sin = static_cast<float>(sin(line_angle)),
		      angle_cos = static_cast<float>(cos(line_angle)),
		      deltaXA = way.a.width * angle_sin,
		      deltaXB = way.b.width * angle_sin,
		      deltaYA = way.a.width * angle_cos,
		      deltaYB = way.b.width * angle_cos;

		Segment2 upper{line.a.x + deltaXA, line.a.y - deltaYA,
		               line.b.x + deltaXB, line.b.y - deltaYB},
		    lower{line.b.x - deltaXB, line.b.y + deltaYB, line.a.x - deltaXA,
		          line.a.y + deltaYA};

		for (auto& wall : wallsP) {
			Point2 ipointUpper, ipointLower;
			bool iupper = wall.intersectsWith(upper, &ipointUpper),
			     ilower = wall.intersectsWith(lower, &ipointLower);

			if (iupper xor ilower) {
				Segment2& iline = iupper ? upper : lower;
				const Point2& cpoint = iupper ? ipointUpper : ipointLower;
				Point2 &thisCloser = iline.getEndCloserTo(cpoint),
				       &otherCloser = wall.getEndCloserTo(cpoint);

				// Cut these lines at the point of intersection
				thisCloser = cpoint;
				otherCloser = cpoint;
			}
		}

		wallsP.push_back(lower);
		wallsP.push_back(upper);
	}

	int i = 0;
	for (const auto& wall : wallsP) {
		points[i++] = wall.a.x;
		points[i++] = wall.a.y;

		points[i++] = wall.b.x;
		points[i++] = wall.b.y;
	}

	VertexArray va(&(points[0]), i);
	va.build(2, false);

	return {paths, wallsP, va};
}

void Maze::draw(GLenum mode) { vertArray.draw(mode); }
