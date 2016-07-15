#include "Maze.h"

#include "geometry_io.h"

#include <cassert>
#include <cmath>

#define MAX_LINES 30
#define PI 3.141592653589793238462643383279502884L

#include <iostream>
using std::cout;

bool tryToInsert(CrossRoad cr, WidePoint2 point_to_insert)
{
	for (const auto& point : cr.points) {
		float dist =
		          Segment2{point.point, point_to_insert.point}.calcSquaredLen(),
		      max_allowed = static_cast<float>(point.width * point.width +
		                                       point_to_insert.width *
		                                           point_to_insert.width);
		if (dist <= max_allowed) {
			cr.points.push_back(point_to_insert);
			puts("Push back two points that are near!");
			return true;
		}
		cout << point << ' ' << point_to_insert << " Dist = " << dist
		     << ", max allowed: " << max_allowed << '\n';
		/* printf("dist = %f, max_allowed: %f\n", dist, max_allowed); */
	}
	return false;
}

Maze Maze::fromPaths(Ways paths)
{
	Lines wallsP, wallsS; // primary and secondary

	std::vector<CrossRoad> cross_roads;

	for (const auto& way : paths) {
		puts("\n\nNEXT INSERT START");
		const Segment2 line = way.getSegmnet2();
		double line_angle = atan((line.b.y - line.a.y) / (line.b.x - line.a.x));
		float angle_sin = static_cast<float>(sin(line_angle)),
		      angle_cos = static_cast<float>(cos(line_angle)),
		      deltaXA = way.a.width * angle_sin,
		      deltaXB = way.b.width * angle_sin,
		      deltaYA = way.a.width * angle_cos,
		      deltaYB = way.b.width * angle_cos;

		Segment2 upper{line.a.x + deltaXA, line.a.y - deltaYA, nullptr,
		               line.b.x + deltaXB, line.b.y - deltaYB, nullptr},
		    lower{line.b.x - deltaXB, line.b.y + deltaYB, nullptr,
		          line.a.x - deltaXA, line.a.y + deltaYA, nullptr};

		bool modified = false;

		for (auto& wall : wallsP) {
			puts("\nCHECK WITH ANOTHER WALL");
			Point2 ipointUpper, ipointLower;
			bool iupper = wall.intersectsWith(upper, &ipointUpper),
			     ilower = wall.intersectsWith(lower, &ipointLower);

			if (iupper xor ilower) {
				puts("Just one intersect point!");
				std::cout << "other: " << wall << "\n";
				Segment2& iline = iupper ? upper : lower;
				const Point2& cpoint = iupper ? ipointUpper : ipointLower;
				Point2 &thisCloser = iline.getEndCloserTo(cpoint),
				       &otherCloser = wall.getEndCloserTo(cpoint);
				std::cout << "Other closer: " << otherCloser << std::endl;
				std::cout << "Line which we're adding: " << way << std::endl;
				std::cout << "Point which we're adding: " << way.b << std::endl;

				assert(otherCloser.crossRoad != nullptr);

				if (tryToInsert(*otherCloser.crossRoad, way.a)) {
					// Cut these lines at the point of intersection
					thisCloser = cpoint;
					otherCloser = cpoint;
					modified = true;
					CrossRoad to_b{{}, {way.b}};
					auto size = wallsP.size();
					wallsP.push_back(lower);
					wallsP.push_back(upper);

					auto croad_count = cross_roads.size();
					cross_roads.push_back(to_b);
					cross_roads[croad_count].lines.push_back(&wallsP[size]);
					cross_roads[croad_count].lines.push_back(&wallsP[size + 1]);

					wallsP[size].b.crossRoad = &cross_roads[croad_count];
					wallsP[size + 1].b.crossRoad = &cross_roads[croad_count];
				}

				else if (tryToInsert(*otherCloser.crossRoad, way.b)) {
					// Cut these lines at the point of intersection
					thisCloser = cpoint;
					otherCloser = cpoint;
					modified = true;
					CrossRoad to_a{{}, {way.a}};
					auto size = wallsP.size();
					wallsP.push_back(lower);
					wallsP.push_back(upper);

					auto croad_count = cross_roads.size();
					cross_roads.push_back(to_a);
					cross_roads[croad_count].lines.push_back(&wallsP[size]);
					cross_roads[croad_count].lines.push_back(&wallsP[size + 1]);

					wallsP[size].a.crossRoad = &cross_roads[croad_count];
					wallsP[size + 1].a.crossRoad = &cross_roads[croad_count];
				}
			}
		}

		if (!modified) {
			puts("\nnot modified!");
			auto wall_count = wallsP.size();
			wallsP.push_back(lower);
			wallsP.push_back(upper);

			CrossRoad to_a{{&wallsP[wall_count], &wallsP[wall_count + 1]},
			               {way.a}};
			CrossRoad to_b{{&wallsP[wall_count], &wallsP[wall_count + 1]},
			               {way.b}};

			auto cross_road_count = cross_roads.size();
			cross_roads.push_back(to_a);
			cross_roads.push_back(to_b);

			wallsP[wall_count].b.crossRoad = &cross_roads[cross_road_count];
			wallsP[wall_count].a.crossRoad = &cross_roads[cross_road_count + 1];

			wallsP[wall_count + 1].b.crossRoad =
			    &cross_roads[cross_road_count + 1];
			wallsP[wall_count + 1].a.crossRoad = &cross_roads[cross_road_count];
		} else {
			puts("it was modified! should break!");
			/* break; */
		}
	}

	assert(wallsP.size() <= MAX_LINES);
	float points[MAX_LINES];

	int i = 0;
	for (const auto& wall : wallsP) { // Copy wallsP into points[]
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
