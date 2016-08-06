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

// @TODO won't write full types now!
void add_a_single_way_to_maze(auto& wallsP, auto& way, auto& colors,
                              auto& color_count, auto& color, auto& cross_roads)
{
	if (!check_lines_for_cross_roads(wallsP)) {
		throw "SOMETHING'S NOT RIGHT. TERMINATE!";
	}

	const Segment2 line = way.getSegmnet2();
	double line_angle = atan((line.b.y - line.a.y) / (line.b.x - line.a.x));
	float angle_sin = static_cast<float>(sin(line_angle)),
	      angle_cos = static_cast<float>(cos(line_angle)),
	      deltaXA = way.a.width * angle_sin, deltaXB = way.b.width * angle_sin,
	      deltaYA = way.a.width * angle_cos, deltaYB = way.b.width * angle_cos;

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
			const Point2& cpoint = iupper ? ipointUpper : ipointLower;
			Point2 &thisCloser =
			           (iupper ? upper : lower).getEndCloserTo(cpoint),
			       &otherCloser = wall.getEndCloserTo(cpoint);
			std::cout << "Just one intersect point! " << cpoint
			          << "Other closer point: " << otherCloser << std::endl;

			if (nullptr == otherCloser.crossRoad) {
				throw "Point doesn't have crossRoad!";
			}
			if (otherCloser.crossRoad->points.size() < 1) {
				throw "Other closer's crossRoad has no points!";
			}

			std::cout << "Point which we're trying to add (way.a): " << way.a
			          << std::endl;
			if (tryToInsert(*otherCloser.crossRoad, way.a)) {
				std::cout << "Successfully instered this point!" << std::endl;
				// Cut these lines at the point of intersection
				thisCloser = cpoint;
				otherCloser.x = cpoint.x; // this way we keep the
				otherCloser.y = cpoint.y; // crossRoad's pointers

				CrossRoad to_b{{}, {way.b}};
				cross_roads.push_back(to_b);

				if (!modified) {
					wallsP.push_back(lower);
					wallsP.push_back(upper);
				}

				Segment2 &lower_in_walls = *std::prev(wallsP.end(), 2),
				         &upper_in_walls = wallsP.back();

				cross_roads.back().lines.push_back(&lower_in_walls);
				lower_in_walls.b.crossRoad = &cross_roads.back();

				cross_roads.back().lines.push_back(&upper_in_walls);
				upper_in_walls.b.crossRoad = &cross_roads.back();

				upper_in_walls.a.crossRoad = otherCloser.crossRoad;
				lower_in_walls.a.crossRoad = otherCloser.crossRoad;

				modified = true;
			}

			printf("Will check lines for crossroads...\n");
			if (!check_lines_for_cross_roads({wallsP.back()}))
				throw "FIX ME 912347892 :)";

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
}

// @TODO full types
int put_walls_and_paths_to_vertex_points(float points[], auto& wallsP,
                                         auto& paths)
{
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

	for (const auto& path : paths) {
		points[i++] = path.a.point.x;
		points[i++] = path.a.point.y;
		points[i++] = 0;
		points[i++] = 0.7f;
		points[i++] = 0.7f;
		points[i++] = 0.7f;

		points[i++] = path.b.point.x;
		points[i++] = path.b.point.y;
		points[i++] = 0;
		points[i++] = 0.7f;
		points[i++] = 0.7f;
		points[i++] = 0.7f;
	}
	return i;
}

Maze Maze::fromPaths(Ways paths)
{
	std::list<Segment2> wallsP, wallsS; // primary and secondary

	std::list<CrossRoad> cross_roads;

	std::vector<Color> colors = {
	    {1, 1, 0, "yellow"},       {0.5, .5, .5, "silver"},
	    {1, 1, 1, "white"},        {1, 0, 0, "red"},
	    {0, 1, 0, "green"},        {0, 0, 1, "blue"},
	    {1, 0.5, 0, "orange"},     {0.5, 0, 0.8, "violet"},
	    {0.4, 0.4, 1, "lightblue"}, {0, 0.5, 0, "darkgreen"}};
	auto color_count = colors.size(), color = 0lu;

	int paths_inserted = 0;
	for (const auto& way : paths) {
		add_a_single_way_to_maze(wallsP, way, colors, color_count, color,
		                         cross_roads);

		if (++paths_inserted > 10) {
			puts("NOT ALLOWED TO INSERT MORE THAN 10 PATHS!");
			break;
		}
	}

	printf("\n%lu walls generated", wallsP.size());
	if (wallsP.size() > MAX_LINES) {
		printf(", allowed at most %d\n", MAX_LINES);
		throw - 1;
	}
	printf(" from %lu lines\n", paths.size());

	if (!check_lines_for_cross_roads(wallsP)) {
		throw "FIX ME 1764202438082";
	}

	// 12 floats per line: per point 3x position and 3x color
	float points[MAX_LINES * 12];

	int vertex_count =
	    put_walls_and_paths_to_vertex_points(points, wallsP, paths);
	VertexArray va(&(points[0]), vertex_count);
	va.build(3, true);

	return {paths, wallsP, va};
}

void Maze::draw(GLenum mode) { vertArray.draw(mode); }
