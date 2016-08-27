#include "Maze.h"

#include "geometry_io.h"

#include <cassert>
#include <cmath>

#define MAX_LINES 30
#define PI 3.141592653589793238462643383279502884L

#include <iostream>
using std::cout;

bool tryToInsert(CrossRoad& cr, WidePoint2 point_to_insert)
{
	if (cr.points.size() < 1)
		throw "CrossRoad MUST have at least 1 point inside!";

	for (const auto& point : cr.points) {
		float dist = calcSquaredLen(point.point, point_to_insert.point),
		      max_allowed = pythagoras(point.width, point_to_insert.width);
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

bool check_lines_for_cross_roads(std::list<WallSegment2> walls)
{
	int i = 0;

	// NOTE the implicit cast from WallSegment2 to Segment2 (via the operator
	// Segment2  function)
	for (Segment2 el : walls) {
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
	         lower = {line.a.x - deltaXA,
	                  line.a.y + deltaYA,
	                  nullptr,
	                  line.b.x - deltaXB,
	                  line.b.y + deltaYB,
	                  nullptr,
	                  colors[(color + 1) % color_count]};

	printf("\nNEXT INSERT START: %s/%s\n", upper.color.name, lower.color.name);

	bool modifiedA = false, modifiedB;

	for (WallSegment2& wall_segment : wallsP) {
		Segment2& wall = wall_segment.segment;
		std::cout << "CHECK WITH " << wall.color.name << " " << wall << ":";
		Point2 ipointUpper, ipointLower;
		const bool iupper = wall.intersectsWith(upper, &ipointUpper),
		           ilower = wall.intersectsWith(lower, &ipointLower);

		if (iupper && ilower) {
			puts("BOTH INTERSECT!");
		} else if (iupper xor ilower) {
			puts("Just one intersect point!");
			// Disable stdout, no need for it now. Enabled again at end of `if`
			/* std::cout.setstate(std::ios_base::failbit); */
			const Point2& cross_point = iupper ? ipointUpper : ipointLower;
			Point2 &thisCloser =
			           (iupper ? upper : lower).getEndCloserTo(cross_point),
			       &otherCloser = wall.getEndCloserTo(cross_point);
			std::cout << "CrossPoint: " << cross_point
			          << "Other closer point: " << otherCloser
			          << " Other closer crossRoad first point: "
			          << otherCloser.crossRoad->points[0] << std::endl;

			if (nullptr == otherCloser.crossRoad) {
				throw "Point doesn't have crossRoad!";
			}
			if (otherCloser.crossRoad->points.size() < 1) {
				throw "Other closer's crossRoad has no points!";
			}

			bool way_a_was_just_added = false;
			{
				std::cout << "Point which we're trying to add (way.a): "
				          << way.a << std::endl;
				if (tryToInsert(*otherCloser.crossRoad, way.a)) {
					std::cout << "Successfully instered this point!"
					          << std::endl;
					// Cut these lines at the point of intersection
					thisCloser.x = cross_point.x;
					thisCloser.y = cross_point.y;
					otherCloser.x = cross_point.x; // this way we keep the
					otherCloser.y = cross_point.y; // crossRoad's pointers

					CrossRoad to_b{{}, {way.b}};
					cross_roads.push_back(to_b);

					cross_roads.back().lines.push_back(&lower);
					lower.b.crossRoad = &cross_roads.back();

					cross_roads.back().lines.push_back(&upper);
					upper.b.crossRoad = &cross_roads.back();

					upper.a.crossRoad = otherCloser.crossRoad;
					lower.a.crossRoad = otherCloser.crossRoad;

					modifiedA = true;
					way_a_was_just_added = 1;
				}
			}

			if (!way_a_was_just_added) {
				std::cout << "Point which we're trying to add (way.b): "
				          << way.b << std::endl;
				if (tryToInsert(*otherCloser.crossRoad, way.b)) {
					std::cout << "Successfully instered this point!"
					          << std::endl;
					// Cut these lines at the point of intersection
					/* thisCloser = cross_point; */
					thisCloser.x = cross_point.x;
					thisCloser.y = cross_point.y;
					otherCloser.x = cross_point.x; // this way we keep the
					otherCloser.y = cross_point.y; // crossRoad's pointers

					CrossRoad to_a{{}, {way.a}};
					cross_roads.push_back(to_a);

					cross_roads.back().lines.push_back(&upper);
					upper.a.crossRoad = &cross_roads.back(); // switch a & b

					cross_roads.back().lines.push_back(&lower);
					lower.a.crossRoad = &cross_roads.back(); // switch a & b

					lower.b.crossRoad = otherCloser.crossRoad; // switch a & b
					upper.b.crossRoad = otherCloser.crossRoad; // switch a & b

					modifiedB = true;
				}
			}

			/* puts("Will check lines for crossroads..."); */
			if (!check_lines_for_cross_roads({wallsP.back()}))
				throw "FIX ME 912347892 :)";

			/* std::cout.clear(); */
		} else
			puts("They don't intersect!");
	}

	if (!modifiedA && !modifiedB) {
		/* puts("\nnot modified!"); */

		// ATTENTION! lower IS REPLACED WITH THE ONE IN wallsP!
		wallsP.push_back({lower, nullptr});
		WallSegment2& lower = wallsP.back();

		wallsP.push_back({upper, nullptr});
		WallSegment2& upper = wallsP.back();

		lower.opposite = &upper.segment;
		upper.opposite = &lower.segment;

		CrossRoad to_a = {{&lower.segment, &upper.segment}, {way.a}},
		          to_b = {{&lower.segment, &upper.segment}, {way.b}};

		cross_roads.push_back(to_a);
		lower.segment.a.crossRoad = &cross_roads.back();
		upper.segment.a.crossRoad = &cross_roads.back();

		cross_roads.push_back(to_b);
		lower.segment.b.crossRoad = &cross_roads.back();
		upper.segment.b.crossRoad = &cross_roads.back();

	} else {
		/* puts("\nit was modified!"); */
		wallsP.push_back({lower, nullptr});
		WallSegment2& lower = wallsP.back();

		wallsP.push_back({upper, nullptr});
		WallSegment2& upper = wallsP.back();

		lower.opposite = &upper.segment;
		upper.opposite = &lower.segment;
	}
	check_lines_for_cross_roads(wallsP);
	color = (color + 2) % color_count;
}

// @TODO full types
int put_walls_and_paths_to_vertex_points(float points[], auto& wallsP,
                                         auto& paths)
{
	int i = 0;
	for (const Segment2& wall : wallsP) { // Copy wallsP into points[]
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
	std::list<WallSegment2> wallsP, wallsS; // primary and secondary

	std::list<CrossRoad> cross_roads;

	std::vector<Color> colors = {
	    {1, 1, 0, "yellow"}, //- {.5f, .5f, .5f, "silver"},
	    {1, 1, 1, "white"},           {1, 0, 0, "red"},
	    {0, 1, 0, "green"},           {0, 0, 1, "blue"},
	    {1, 0.5f, 0, "orange"},       {0.5f, 0, 0.8f, "violet"},
	    {0.4f, 0.4f, 1, "lightblue"}, {0, 0.5f, 0, "darkgreen"}};

	auto color_count = colors.size(), color = 0lu;

	freopen("out.txt", "w", stdout);
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

	for (const Segment2& p : wallsP) {
		std::cout << p.color.name << std::endl;
		std::cout << p.a << " : ";
		for (auto c : p.a.crossRoad->lines) {
			std::cout << *c << ", ";
		}
		std::cout << std::endl;
		std::cout << p.b << " : ";
		for (auto c : p.b.crossRoad->lines) {
			std::cout << *c << ", ";
		}
		std::cout << std::endl;
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
