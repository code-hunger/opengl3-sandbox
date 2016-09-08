#include "graphics/geometry_io.h"

#include <cassert>
#include <cmath>
#include <list>
#include <unordered_set>

#define PI 3.141592653589793238462643383279502884L
#define FORCE_VALIDATE false

#include <iostream>
using std::cout;
using std::endl;

typedef std::unordered_set<WideRoad2, Hash> Ways;
typedef std::list<Wall2> Walls;
typedef std::vector<Color> Colors;
typedef std::list<CrossRoad> CrossRoads;

template <typename T> auto get_short_addr(const T& var)
{
	char addr[20];
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
	sprintf(addr, "%llu", &var);
#pragma GCC diagnostic pop
	return atoll(addr) % 10000;
}

bool tryToInsert(CrossRoad& cr, WidePoint2 point_to_insert)
{
	if (cr.points.size() < 1)
		throw "CrossRoad MUST have at least 1 point inside!";

	constexpr bool DEBUG = false;

	for (const auto& point : cr.points) {
		float dist = calcSquaredLen(point.point, point_to_insert.point),
		      max_allowed = pythagoras(point.width, point_to_insert.width);
		DEBUG&& cout << point << ' ' << point_to_insert << " Dist = " << dist
		             << ", max allowed: " << max_allowed << '\n';
		if (dist <= max_allowed) {
			cr.points.push_back(point_to_insert);
			DEBUG&& puts("Push back two points that are near!");
			return true;
		}
	}
	return false;
}

void validate_cross_road(CrossRoad* crossRoad)
{
	if (crossRoad == nullptr) {
		throw "CrossRoad pointer is empty!";
	}
	if (crossRoad->points.size() < 1) {
		throw "CrossRoads should have at least one point!";
	}
	if (crossRoad->lines.size() < 2) {
		throw "CrossRoads can't have less than 2 lines!";
	}
}

void validate_walls(Walls& walls, bool force = FORCE_VALIDATE)
{
	if (!force) {
		return;
	}
	cout << "Validating walls...";
	bool valid = true;
	for (const auto& wall : walls) {
		// First check the opposite
		if (wall.opposite == nullptr) {
			cout << "\nOpposite is nullptr: " << wall.segment << endl;
			valid = false;
		} else if (wall.opposite->opposite != &wall) {
			cout << "\nThe opposite's opposite is not the current wall!";
			valid = false;
		} else {
			if (wall.segment.a.crossRoad !=
			    wall.opposite->segment.a.crossRoad) {
				puts("\npointA's crossRoad is not the same as opposite's "
				     "A.crossRoad");
				valid = false;
			}
			if (wall.segment.b.crossRoad !=
			    wall.opposite->segment.b.crossRoad) {
				std::cout << "\npointB's crossRoad is not the same as "
				             "opposite's B.crossRoad"
				          << endl;
				valid = false;
			}
		}

		try {
			validate_cross_road(wall.segment.a.crossRoad);
			validate_cross_road(wall.segment.b.crossRoad);
		} catch (const char* err) {
			cout << "\n" << err << endl;
			valid = false;
		}
	}
	if (!valid) {
		cout << endl;
		throw "Walls' state invalid!";
	}
	cout << " ready!" << endl;
}

void one_intersect_point(bool iupper, const Ways::value_type& way,
                         Point2& ipointUpper, Point2& ipointLower, Wall2& upper,
                         Wall2& lower, Wall2& wall, CrossRoads& cross_roads)
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

		lower.segment.b.crossRoad = upper.segment.b.crossRoad =
		    &cross_roads.back();
		upper.segment.a.crossRoad = lower.segment.a.crossRoad =
		    otherCloser.crossRoad;
	} else {
		cout << "Point which we're trying to add (way.b): " << way.b << endl;
		if (tryToInsert(*otherCloser.crossRoad, way.b)) {
			cout << "Successfully instered this point!\n";
			// Cut these lines at the point of intersection
			thisCloser.moveTo(cross_point);
			otherCloser.moveTo(cross_point);

			upper.segment.a.crossRoad = lower.segment.a.crossRoad =
			    &cross_roads.back();
			lower.segment.b.crossRoad = upper.segment.b.crossRoad =
			    otherCloser.crossRoad;
		}
	}
}

void two_intersect_points(Wall2& wall, WideRoad2 way, Wall2& upper,
                          Wall2& lower, Walls& walls, Colors& colors,
                          Colors::size_type& color,
                          Colors::size_type& color_count, Point2& ipointUpper,
                          Point2& ipointLower)
{
	Point2 ipointUpperOpposite{}, ipointLowerOpposite{};

	// if upper or lower crosses the opposite of wall
	const bool iupperOpposite = wall.opposite->segment.intersectsWith(
	               upper.segment, &ipointUpperOpposite),
	           ilowerOpposite = wall.opposite->segment.intersectsWith(
	               lower.segment, &ipointLowerOpposite);

	if (iupperOpposite && ilowerOpposite) {
		cout << "The opposite is crossed too by both lower and upper!!" << endl;
	} else {
		Point2 cpoint_of_A =
		    Segment2{ipointUpper, ipointLower}.getEndCloserTo(wall.segment.a);
		Point2 cpoint_of_B =
		    Segment2{ipointUpper, ipointLower}.getEndCloserTo(wall.segment.b);
		(void)cpoint_of_A;
		(void)cpoint_of_B;

		CrossRoad* crossRoad =
		    // upper and lower should have the same crossRoad
		    lower.segment.getEndCloserTo(ipointLower).crossRoad;
		validate_cross_road(crossRoad);

		Point2 &otherCloserToiLower = wall.segment.getEndCloserTo(ipointLower),
		       &otherCloserToiUpper = wall.segment.getEndCloserTo(ipointUpper);

		bool cut_upper = calcSquaredLen(otherCloserToiLower, ipointLower) <
		                 calcSquaredLen(otherCloserToiUpper, ipointUpper);
		Point2& otherCloser =
		    cut_upper ? otherCloserToiUpper : otherCloserToiLower;

		if (tryToInsert(*otherCloser.crossRoad, way.a) ||
		    tryToInsert(*otherCloser.crossRoad, way.b)) {
			auto &cut_point = cut_upper ? ipointUpper : ipointLower,
			     &ncut_point = !cut_upper ? ipointUpper : ipointLower;
			auto &cut_wall = cut_upper ? upper.segment : lower.segment,
			     &ncut_wall = cut_upper ? lower.segment : upper.segment;

			otherCloser.moveTo(cut_point);

			cut_wall.getEndCloserTo(cut_point).moveTo(cut_point).crossRoad =
			    otherCloser.crossRoad;

			ncut_wall.getEndCloserTo(ncut_point).crossRoad =
			    otherCloser.crossRoad;
			return;
		}

		Segment2 _otherUpper = {wall.segment.a, ipointLower,
		                        colors[(color) % color_count]},
		         _otherLower = {wall.opposite->segment.a, ipointUpperOpposite,
		                        colors[(color + 1) % color_count]};
		color = (color + 2) % color_count;

		_otherUpper.b.crossRoad = crossRoad;
		_otherLower.b.crossRoad = crossRoad;

		walls.push_back({_otherUpper});
		Wall2& otherUpper = walls.back();
		walls.push_back({_otherLower});
		Wall2& otherLower = walls.back();

		otherUpper.opposite = &otherLower;
		otherLower.opposite = &otherUpper;

		// Move the A points to the point which is closer to B
		wall.segment.a.moveTo(cpoint_of_B);

		wall.opposite->segment.a.moveTo(ipointUpperOpposite);

		wall.segment.a.crossRoad = crossRoad;
		wall.opposite->segment.a.crossRoad = crossRoad;

		// cut lower and upper to the point of intersection
		lower.segment.getEndCloserTo(ipointLower).moveTo(ipointLower);
		upper.segment.getEndCloserTo(ipointUpper).moveTo(ipointUpper);
	}
}

void add_a_single_way_to_maze(Walls& wallsP, const Ways::value_type& way,
                              Colors& colors, Colors::size_type& color_count,
                              Colors::size_type& color, CrossRoads& cross_roads)
{
	validate_walls(wallsP);

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
	color = (color + 2) % color_count;

	wallsP.push_back({_upper});
	Wall2& upper = wallsP.back();

	wallsP.push_back({_lower});
	Wall2& lower = wallsP.back();

	upper.opposite = &lower;
	lower.opposite = &upper;

	cross_roads.push_back({{&lower, &upper}, {way.a}});
	lower.segment.a.crossRoad = upper.segment.a.crossRoad = &cross_roads.back();

	cross_roads.push_back({{&lower, &upper}, {way.b}});
	lower.segment.b.crossRoad = upper.segment.b.crossRoad = &cross_roads.back();

	printf("\nNEXT INSERT START: %s/%s\n", upper.segment.color.name,
	       lower.segment.color.name);

	for (Wall2& wall : wallsP) {
		if (&wall == &upper || &wall == &lower) continue;
		Segment2 &segment = wall.segment, &opposite = wall.opposite->segment;
		cout << "CHECK WITH " << segment << ":";
		Point2 ipointUpper{}, ipointLower{};
		const bool iupper = segment.intersectsWith(upper.segment, &ipointUpper),
		           ilower = segment.intersectsWith(lower.segment, &ipointLower);

		if (iupper && ilower) {
			puts("BOTH INTERSECT!");
			two_intersect_points(wall, way, upper, lower, wallsP, colors, color,
			                     color_count, ipointUpper, ipointLower);
		} else {
			bool iupperOpposite =
			         opposite.intersectsWith(lower.segment, nullptr),
			     ilowerOpposite =
			         opposite.intersectsWith(lower.segment, nullptr);

			if (iupperOpposite && ilowerOpposite) {
				puts("BOTH INTERSECT (with wall's opposite, skip it now)");
			} else if (iupper xor ilower) {
				puts("Just one intersect point!");
				one_intersect_point(iupper, way, ipointUpper, ipointLower,
				                    upper, lower, wall, cross_roads);
			} else {
				puts("---");
			}
		}
	}

	validate_walls(wallsP);
}

void build_from_paths(const Ways& paths, Walls& wallsP)
{
	// Walls wallsP;
	CrossRoads cross_roads;
	Colors colors = {{1, 1, 0, "yellow"},          {.5f, .5f, .5f, "silver"},
	                 {1, 1, 1, "white"},           {1, 0, 0, "red"},
	                 {0, 1, 0, "green"},           {0, 0, 1, "blue"},
	                 {1, 0.5f, 0, "orange"},       {0.5f, 0, 0.8f, "violet"},
	                 {0.4f, 0.2f, 1, "lightblue"}, {0, 0.5f, 0, "darkgreen"}};

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
	validate_walls(wallsP, true);

	printf("\n%lu walls generated from %lu lines\n", wallsP.size(),
	       paths.size());

	for (const auto& _p : wallsP) {
		auto p = _p.segment;
		cout << p.color.name << endl
		     << p.a << "(" << get_short_addr(*p.a.crossRoad) << ") : ";
		for (const auto& c : p.a.crossRoad->points) {
			cout << c << ", ";
		}
		cout << endl << p.b << "(" << get_short_addr(*p.b.crossRoad) << ") : ";
		for (const auto& c : p.b.crossRoad->points) {
			cout << c << ", ";
		}
		cout << endl;
	}
}
