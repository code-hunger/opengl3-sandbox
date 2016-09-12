#include "def.h"
#include "graphics/geometry_io.h"
#include "join_lines.h"
#include "validator.h"

#include <cassert>
#include <cmath>
#define PI 3.141592653589793238462643383279502884L

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

inline bool tryToInsert(CrossRoad& cr, WideRoad2 way)
{
	return tryToInsert(cr, way.a) || tryToInsert(cr, way.b);
}

void one_intersect_point(bool iupper, const WideRoad2& way, Point2& ipointUpper,
                         Point2& ipointLower, Wall2& upper, Wall2& lower,
                         Wall2& wall)
{
	const Point2& cross_point = iupper ? ipointUpper : ipointLower;

	bool join_at_a = (&way.getEndCloserTo(cross_point) == &way.a);

	if (tryToInsert(*wall.segment.getEndCloserTo(cross_point).crossRoad,
	                join_at_a ? way.a : way.b)) {
		join_end_end(wall, iupper ? upper : lower, cross_point, join_at_a);
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
		return;
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

		if (tryToInsert(*otherCloser.crossRoad, way)) {
			cout << "Successfull trial to insert" << endl;
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

		// join_end_middle()

		if (1) {
			Segment2 _otherUpper = {wall.segment.a, ipointLower, colors[color]},
			         _otherLower = {wall.opposite->segment.a,
			                        ipointUpperOpposite,
			                        colors[(color + 1) % color_count]};
			color = (color + 2) % color_count;
			color = color == 0 ? 1 : (color == 1 ? 0 : color);

			_otherUpper.b.crossRoad = crossRoad;
			_otherLower.b.crossRoad = crossRoad;

			walls.push_back({_otherUpper});
			Wall2& otherUpper = walls.back();
			walls.push_back({_otherLower});
			Wall2& otherLower = walls.back();

			otherUpper.disable_intersect_check = true;
			otherLower.disable_intersect_check = true;

			otherUpper.opposite = &otherLower;
			otherLower.opposite = &otherUpper;
		}

		// Move the A points to the point which is closer to B
		wall.segment.a.moveTo(cpoint_of_B);

		wall.opposite->segment.a.moveTo(ipointUpperOpposite);

		wall.segment.a.crossRoad = crossRoad;
		wall.opposite->segment.a.crossRoad = crossRoad;

		// cut lower and upper to the point of intersection
		if (calcSquaredLen(upper.segment.a, ipointUpper) <
		    calcSquaredLen(upper.segment.a, ipointUpperOpposite)) {
			upper.segment.b.moveTo(ipointUpper);
		} else {
			upper.segment.b.moveTo(ipointUpperOpposite);
		}

		if (calcSquaredLen(lower.segment.a, ipointLower) <
		    calcSquaredLen(lower.segment.a, ipointLowerOpposite)) {
			lower.segment.b.moveTo(ipointLower);
		} else {
			lower.segment.b.moveTo(ipointLowerOpposite);
		}
	}
}

void add_a_single_way_to_maze(Walls& wallsP, const WideRoad2& way,
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
		if (wall.disable_intersect_check) {
			wall.disable_intersect_check = 0;
			continue;
		};
		Segment2 &segment = wall.segment, &opposite = wall.opposite->segment;
		cout << "CHECK WITH " << segment.color.name << ":";
		Point2 ipointUpper{}, ipointLower{};
		const bool iupper = segment.intersectsWith(upper.segment, &ipointUpper),
		           ilower = segment.intersectsWith(lower.segment, &ipointLower);

		if (iupper && ilower) {
			puts("BOTH INTERSECT!");

			two_intersect_points(wall, way, upper, lower, wallsP, colors, color,
			                     color_count, ipointUpper, ipointLower);
		} else {
			bool iupperOpposite =
			         opposite.intersectsWith(upper.segment, nullptr),
			     ilowerOpposite =
			         opposite.intersectsWith(lower.segment, nullptr);

			if (iupperOpposite && ilowerOpposite) {
				puts("BOTH INTERSECT (with wall's opposite, skip it now)");
				/*two_intersect_points(wall, way, upper, lower, wallsP, colors,
				                     color, color_count, ipointUpper,
				                     ipointLower);*/
			} else if (iupper xor ilower) {
				puts("Just one intersect point!");
				one_intersect_point(iupper, way, ipointUpper, ipointLower,
				                    upper, lower, wall);
			} else {
				puts("---");
			}
		}
	}

	validate_walls(wallsP);
}

void build_from_paths(const Ways& paths, Walls& walls)
{
	CrossRoads cross_roads;
	Colors colors = {{1, 1, 0.1f, "L yellow"},    {0, 0.3f, 1.f, "D blue"},
	                 {0.5f, 0, 0.8f, "violet"},   {1, 0.5f, 0, "orange"},
	                 {.5f, .5f, .5f, "silver"},   {1.f, 0, 0, "red"},
	                 {1.f, 0.5f, 0.5f, "pink"},   {0, .7f, 1.f, "L blue"},
	                 {0.7f, 0.2f, 0.3f, "brown"}, {0.2f, 1, 0.2f, "L green"},
	                 {0, 0.5f, 0, "D green"}};

	auto color_count = colors.size(), color = 0lu;

	for (const auto& way : paths) {
		add_a_single_way_to_maze(walls, way, colors, color_count, color,
		                         cross_roads);
	}

	validate_walls(walls, true);

	printf("\n%lu walls generated from %lu lines\n", walls.size(),
	       paths.size());

	dump(walls);
	cout << "\nCrossRoads in std::list cross_roads: " << cross_roads.size()
	     << endl;
}
