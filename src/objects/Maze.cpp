#include "Maze.h"

#include "graphics/geometry_io.h"

#include <cassert>
#include <cmath>

#define MAX_LINES 30
#define PI 3.141592653589793238462643383279502884L

#include <iostream>
using std::cout;
using std::endl;

typedef std::vector<Color> Colors;
typedef std::list<CrossRoad> CrossRoads;

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

bool check_lines_for_cross_roads(Walls walls)
{
	int i = 0;

	for (const auto& el : walls) {
		auto a_cross = el.segment.a.crossRoad, b_cross = el.segment.b.crossRoad;
		if (a_cross == nullptr || b_cross == nullptr) {
			cout << "element " << i << ": " << el.segment << endl;
			throw "crossRoad is NULLPTR!!";
		}
		if (a_cross->points.size() < 1 || b_cross->points.size() < 1) {
			cout << "element " << i << ": " << el.segment << endl;
			throw "both cross points must have at least one point!";
		}
		++i;
	}
	puts("LINES CHECK passed!");
	return true;
}

void one_intersect_point(bool iupper, const Ways::value_type& way,
                         Point2& ipointUpper, Point2& ipointLower, Wall2& upper,
                         Wall2& lower, Wall2& wall, bool& modifiedA,
                         bool& modifiedB, CrossRoads& cross_roads)
{
	const Point2& cross_point = iupper ? ipointUpper : ipointLower;
	Point2 &thisCloser =
	           (iupper ? upper : lower).segment.getEndCloserTo(cross_point),
	       &otherCloser = wall.segment.getEndCloserTo(cross_point);
	cout << "CrossPoint: " << cross_point
	     << "Other closer point: " << otherCloser
	     << " Other closer crossRoad first point: "
	     << otherCloser.crossRoad->points[0] << endl;

	if (otherCloser.crossRoad == nullptr) {
		throw "Point doesn't have crossRoad!";
	}
	if (otherCloser.crossRoad->points.size() < 1) {
		throw "Other closer's crossRoad has no points!";
	}

	cout << "Point which we're trying to add (way.a): " << way.a << endl;
	if (tryToInsert(*otherCloser.crossRoad, way.a)) {
		cout << "Successfully instered this point!\n";
		// Cut these lines at the point of intersection
		thisCloser.moveTo(cross_point);
		otherCloser.moveTo(cross_point);

		if (!modifiedB) {
			modifiedB = 1;
			CrossRoad to_b{{}, {way.b}};
			cross_roads.push_back(to_b);

			cross_roads.back().lines.push_back(&lower);
			cross_roads.back().lines.push_back(&upper);

			lower.segment.b.crossRoad = upper.segment.b.crossRoad =
			    &cross_roads.back();
			upper.segment.a.crossRoad = lower.segment.a.crossRoad =
			    otherCloser.crossRoad;
		}

		modifiedA = true;

	} else {
		cout << "Point which we're trying to add (way.b): " << way.b << endl;
		if (tryToInsert(*otherCloser.crossRoad, way.b)) {
			cout << "Successfully instered this point!\n";
			// Cut these lines at the point of intersection
			thisCloser.moveTo(cross_point);
			otherCloser.moveTo(cross_point);

			if (!modifiedA) {
				modifiedA = 1;
				CrossRoad to_a{{}, {way.a}};
				cross_roads.push_back(to_a);

				cross_roads.back().lines.push_back(&upper);
				cross_roads.back().lines.push_back(&lower);

				upper.segment.a.crossRoad = lower.segment.a.crossRoad =
				    &cross_roads.back();
				lower.segment.b.crossRoad = upper.segment.b.crossRoad =
				    otherCloser.crossRoad;
			}

			modifiedB = true;
		}
	}
}

void two_intersect_points(Wall2& wall, Wall2& upper, Wall2& lower,
                          Point2& ipointUpper, Point2& ipointLower)
{
	return;
	Point2 ipointUpperOpposite{}, ipointLowerOpposite{};
	const bool iupperOpposite = wall.opposite->intersectsWith(
	               upper.segment, &ipointUpperOpposite),
	           ilowerOpposite = wall.opposite->intersectsWith(
	               lower.segment, &ipointLowerOpposite);

	cout << "iupperOpposite=" << iupperOpposite << ", ilowerOpposite"
	     << ilowerOpposite << endl;
	if (iupperOpposite && ilowerOpposite) {
		cout << "The opposite is crossed too by both lower and upper!!" << endl;
	} else {
		Point2 cpoint_of_A =
		    Segment2{ipointUpper, ipointLower}.getEndCloserTo(wall.segment.a);
		//Point2 cpoint_of_B =
			//Segment2{ipointUpper, ipointLower}.getEndCloserTo(wall.segment.b);

		wall.segment.a.moveTo(cpoint_of_A);
		wall.opposite->a.moveTo(cpoint_of_A);

		lower.segment.getEndCloserTo(ipointLower).moveTo(ipointLower);
		upper.segment.getEndCloserTo(ipointUpper).moveTo(ipointUpper);
	}
}

void add_a_single_way_to_maze(Walls& wallsP, const Ways::value_type& way,
                              Colors& colors, Colors::size_type& color_count,
                              Colors::size_type& color, CrossRoads& cross_roads)
{
	check_lines_for_cross_roads(wallsP);

	const Segment2 line = way.getSegmnet2();
	double line_angle = atan((line.b.y - line.a.y) / (line.b.x - line.a.x));
	float angle_sin = static_cast<float>(sin(line_angle)),
	      angle_cos = static_cast<float>(cos(line_angle)),
	      deltaXA = way.a.width * angle_sin, deltaXB = way.b.width * angle_sin,
	      deltaYA = way.a.width * angle_cos, deltaYB = way.b.width * angle_cos;

	Segment2 _upper = {{line.a.x + deltaXA, line.a.y - deltaYA},
	                   {line.b.x + deltaXB, line.b.y - deltaYB},
	                   colors[color]},
	         _lower = {{line.a.x - deltaXA, line.a.y + deltaYA},
	                   {line.b.x - deltaXB, line.b.y + deltaYB},
	                   colors[(color + 1) % color_count]};

	wallsP.push_back({_upper});
	Wall2& upper = wallsP.back();

	wallsP.push_back({_lower});
	Wall2& lower = wallsP.back();

	cross_roads.push_back({{&lower, &upper}, {way.a}});
	lower.segment.a.crossRoad = upper.segment.a.crossRoad = &cross_roads.back();

	cross_roads.push_back({{&lower, &upper}, {way.b}});
	lower.segment.b.crossRoad = upper.segment.b.crossRoad = &cross_roads.back();

	printf("\nNEXT INSERT START: %s/%s\n", upper.segment.color.name,
	       lower.segment.color.name);

	bool modifiedA{}, modifiedB{};

	for (Wall2& wall : wallsP) {
		Segment2& segment = wall.segment;
		cout << "CHECK WITH " << segment.color.name << " " << segment << ":";
		Point2 ipointUpper{}, ipointLower{};
		const bool iupper = segment.intersectsWith(upper.segment, &ipointUpper),
		           ilower = segment.intersectsWith(lower.segment, &ipointLower);

		if (iupper && ilower) {
			puts("BOTH INTERSECT!");
			two_intersect_points(wall, upper, lower, ipointUpper, ipointLower);
		} else if (iupper xor ilower) {
			puts("Just one intersect point!");
			one_intersect_point(iupper, way, ipointUpper, ipointLower, upper,
			                    lower, wall, modifiedA, modifiedB, cross_roads);
			check_lines_for_cross_roads({wallsP.back()});
		}
	}

	check_lines_for_cross_roads(wallsP);
	color = (color + 2) % color_count;
}

// @TODO full types
int put_walls_and_paths_to_vertex_points(float points[], Walls& wallsP,
                                         Ways& paths)
{
	int i = 0;
	for (const auto& _wall : wallsP) { // Copy wallsP into points[]
		auto wall = _wall.segment;
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
	Walls wallsP, wallsS; // primary and secondary

	CrossRoads cross_roads;

	Colors colors = {{1, 1, 0, "yellow"}, //- {.5f, .5f, .5f, "silver"},
	                 {1, 1, 1, "white"},           {1, 0, 0, "red"},
	                 {0, 1, 0, "green"},           {0, 0, 1, "blue"},
	                 {1, 0.5f, 0, "orange"},       {0.5f, 0, 0.8f, "violet"},
	                 {0.4f, 0.4f, 1, "lightblue"}, {0, 0.5f, 0, "darkgreen"}};

	auto color_count = colors.size(), color = 0lu;

	// freopen("out.txt", "w", stdout);
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

	check_lines_for_cross_roads(wallsP);

	for (const auto& _p : wallsP) {
		auto p = _p.segment;
		cout << p.color.name << endl << p.a << " : ";
		for (const auto& c : p.a.crossRoad->points) {
			cout << c << ", ";
		}
		cout << endl << p.b << " : ";
		for (const auto& c : p.b.crossRoad->points) {
			cout << c << ", ";
		}
		cout << endl;
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
