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
	if (cr.points.size() < 1)
		throw "CrossRoad MUST have at least 1 point inside!";

	for (const auto& point : cr.points) {
		float dist =
		          Segment2{point.point, point_to_insert.point}.calcSquaredLen(),
		      max_allowed = static_cast<float>(point.width * point.width +
		                                       point_to_insert.width *
		                                           point_to_insert.width);
		cout << point << ' ' << point_to_insert << " Dist = " << dist
		     << ", max allowed: " << max_allowed << '\n';
		if (dist <= max_allowed) {
			cr.points.push_back(point_to_insert);
			puts("Push back two points that are near!");
			return true;
		}
	}
	return false;
}

bool check_lines_for_cross_roads(std::list<Segment2> walls)
{
	int i = 0;
	for (auto el : walls) {
		auto a_cross = el.a.crossRoad, b_cross = el.b.crossRoad;
		if (nullptr == a_cross or nullptr == b_cross) {
			std::cout << "element " << i << ": " << el << std::endl;
			throw "crossRoad is NULLPTR!!";
		}
		if (a_cross->points.size() < 1 or b_cross->points.size() < 1) {
			std::cout << "element " << i << ": " << el << std::endl;
			throw "both cross points must have at least one point!";
		}
		++i;
	}
	puts("LINES CHECK passed!");
	return true;
}

Maze Maze::fromPaths(Ways paths)
{
	std::list<Segment2> wallsP, wallsS; // primary and secondary

	std::list<CrossRoad> cross_roads;

	std::vector<Color> colors = {
	    {1, 1, 0, "yellow"}, {0.5, .5, .5, "silver"}, {1, 1, 1, "white"},
	    {1, 0, 0, "red"},    {0, 1, 0, "green"},      {0, 0, 1, "blue"},
	};
	auto color_count = colors.size(), color = 0lu;

	int paths_inserted = 0;
	for (const auto& way : paths) {
		if (!check_lines_for_cross_roads(wallsP)) {
			throw "SOMETHING'S NOT RIGHT. TERMINATE!";
		}

		const Segment2 line = way.getSegmnet2();
		double line_angle = atan((line.b.y - line.a.y) / (line.b.x - line.a.x));
		float angle_sin = static_cast<float>(sin(line_angle)),
		      angle_cos = static_cast<float>(cos(line_angle)),
		      deltaXA = way.a.width * angle_sin,
		      deltaXB = way.b.width * angle_sin,
		      deltaYA = way.a.width * angle_cos,
		      deltaYB = way.b.width * angle_cos;

		Segment2 upper = {line.a.x + deltaXA, line.a.y - deltaYA, nullptr,
		                  line.b.x + deltaXB, line.b.y - deltaYB, nullptr,
		                  colors[color]},
		         lower = {line.b.x - deltaXB,
		                  line.b.y + deltaYB,
		                  nullptr,
		                  line.a.x - deltaXA,
		                  line.a.y + deltaYA,
		                  nullptr,
		                  colors[(color + 1) % color_count]};

		printf("\n\nNEXT INSERT START: %s/%s\n", upper.color.name,
		       lower.color.name);

		bool modified = false;

		for (auto& wall : wallsP) {
			std::cout << "\nCHECK WITH " << wall.color.name << " " << wall
			          << std::endl;
			Point2 ipointUpper, ipointLower;
			bool iupper = wall.intersectsWith(upper, &ipointUpper),
			     ilower = wall.intersectsWith(lower, &ipointLower);

			if (iupper && ilower) {
				puts("BOTH INTERSECT!");
			} else if (iupper xor ilower) {
				Segment2& iline = iupper ? upper : lower;
				const Point2& cpoint = iupper ? ipointUpper : ipointLower;
				Point2 &thisCloser = iline.getEndCloserTo(cpoint),
				       &otherCloser = wall.getEndCloserTo(cpoint);
				std::cout << "Just one intersect point! " << cpoint
				          << std::endl;
				std::cout << "Other closer point: " << otherCloser << std::endl;

				if (nullptr == otherCloser.crossRoad) {
					throw "Point doesn't have crossRoad!";
				}
				if(otherCloser.crossRoad->points.size() < 1) {
					throw "Other closer's crossRoad has no points!";
				}

				std::cout << "Point which we're trying to add (way.a): "
				          << way.a << std::endl;
				if (tryToInsert(*otherCloser.crossRoad, way.a)) {
					std::cout << "Successfully instered this point!" << std::endl;
					// Cut these lines at the point of intersection
					thisCloser = cpoint;
					otherCloser.x = cpoint.x; // this way we keep the 
					otherCloser.y = cpoint.y; // crossRoad's pointers
					modified = true;

					CrossRoad to_b{{}, {way.b}};
					cross_roads.push_back(to_b);

					wallsP.push_back(lower);
					cross_roads.back().lines.push_back(
					    /* lower */ &wallsP.back());

					/* lower */
					wallsP.back().b.crossRoad = &cross_roads.back();
					wallsP.back().a.crossRoad = otherCloser.crossRoad;

					wallsP.push_back(upper);

					cross_roads.back().lines.push_back(
					    /* upper */ &wallsP.back());
					/* upper */
					wallsP.back().b.crossRoad = &cross_roads.back();
					wallsP.back().a.crossRoad = otherCloser.crossRoad;
					std::cout << otherCloser.crossRoad->points.size() << std::endl;
				} else
					puts("First trial failed!");

				printf("Will check lines for crossroads...\n");
				if (!check_lines_for_cross_roads({wallsP.back()}))
					throw "FIX ME 912347892 :)";
				std::cout << "Point which we're trying to add(way.b): " << way.b
				          << std::endl;

				/* if (tryToInsert(*otherCloser.crossRoad, way.b)) { */
				/* 	// Cut these lines at the point of intersection */
				/* 	thisCloser = cpoint; */
				/* 	otherCloser = cpoint; */
				/* 	modified = true; */

				/* 	CrossRoad to_a{{}, {way.a}}; */
				/* 	cross_roads.push_back(to_a); */

				/* 	wallsP.push_back(lower); */
				/* 	cross_roads.back().lines.push_back(&wallsP.back()); */
				/* 	wallsP.back().a.crossRoad = &cross_roads.back(); */

				/* 	wallsP.push_back(upper); */

				/* 	cross_roads.back().lines.push_back(&wallsP.back()); */
				/* 	wallsP.back().a.crossRoad = &cross_roads.back(); */
				/* } else */
				/* 	puts("second trial failed!"); */

			} else
				puts("They don't intersect!");
		}

		if (!modified) {
			puts("\nnot modified!");
			wallsP.push_back(lower);
			Segment2* first = &wallsP.back();

			wallsP.push_back(upper);
			Segment2* second = &wallsP.back();

			CrossRoad to_a{{first, second}, {way.a}};
			CrossRoad to_b{{first, second}, {way.b}};

			cross_roads.push_back(to_a);
			first->b.crossRoad = &cross_roads.back();
			second->a.crossRoad = &cross_roads.back();

			cross_roads.push_back(to_b);
			first->a.crossRoad = &cross_roads.back();
			second->b.crossRoad = &cross_roads.back();

			check_lines_for_cross_roads(wallsP);
		} else {
			puts("\nit was modified!");
		}
		color = (color + 2) % color_count;
		if (++paths_inserted >= 3) break;
	}

	if (wallsP.size() > MAX_LINES) {
		printf("\n%lu walls generated, allowed at most %d\n", wallsP.size(),
		       MAX_LINES);
		throw - 1;
	}

	if(!check_lines_for_cross_roads(wallsP)) {
		throw "FIX ME 1764202438082";
	}

	// 12 floats per line: per point 3x position and 3x color
	float points[MAX_LINES * 12];

	int i = 0;
	for (const auto& wall : wallsP) { // Copy wallsP into points[]
		points[i++] = wall.a.x;
		points[i++] = wall.a.y;
		points[i++] = 0;
		points[i++] = wall.color.r;
		points[i++] = wall.color.g;
		points[i++] = wall.color.b;

		points[i++] = wall.b.x;
		points[i++] = wall.b.y;
		points[i++] = 0;
		points[i++] = wall.color.r;
		points[i++] = wall.color.g;
		points[i++] = wall.color.b;
	}

	VertexArray va(&(points[0]), i);
	va.build(3, true);

	return {paths, wallsP, va};
}

void Maze::draw(GLenum mode) { vertArray.draw(mode); }
