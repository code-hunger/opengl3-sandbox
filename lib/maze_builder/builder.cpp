#include "builder.h"

#include "logger/include/logger.h"
#include "math/include/geometry.h"
#include "math/include/geometry_io.h"

#include <cassert>
#include <cmath>

#include <deque>
#include <list>
#include <set>

constexpr long double PI = 3.141592653589793238462643383279502884L;

constexpr bool FORCE_VALIDATE = false;

constexpr unsigned WIDEPOINT_WIDTH = 3;

using namespace math;

template <typename T> unsigned long ptr(const T& a)
{
	return reinterpret_cast<unsigned long>(&a) % 1000;
}

void insertIfBipgEnough(WideRoads& ways, const WideRoad2& way, float minLength)
{
	if (calcSquaredLen(way) > minLength * minLength) {
		ways.push_back(way);
	}
}

float get_ip_width(const WideRoad2& way, const Point2& ip)
{
	const WidePoint2 &a = (way.a.width < way.b.width ? way.a : way.b),
	                 &b = (&a == &way.a ? way.b : way.a);
	return sqrt(calcSquaredLen(a, ip)) * (b.width - a.width) /
	           sqrt(calcSquaredLen(way)) +
	       a.width;
}

void intersect(WideRoads& ways, WideRoad2& way, WideRoad2& other,
               const Point2& ip, CrossRoads& crossRoads)
{
	WidePoint2 &wayCloser = getEndCloserTo(way, ip),
	           &otherCloser = getEndCloserTo(other, ip);

	const float ipoint_width_way = get_ip_width(way, ip),
	            ipoint_width_other = get_ip_width(other, ip);

	WideRoad2 complementing_to_way = {{ip, ipoint_width_way}, wayCloser},
	          complementing_to_other = {{ip, ipoint_width_other}, otherCloser};

	insertIfBipgEnough(ways, complementing_to_way, way.a.width);
	insertIfBipgEnough(ways, complementing_to_other, other.a.width);

	wayCloser.point = ip;
	wayCloser.width = ipoint_width_way;

	otherCloser.point = ip;
	otherCloser.width = ipoint_width_other;

	WidePoint2 &wayFurther = (&wayCloser == &way.a ? way.b : way.a),
	           &otherFurther = (&otherCloser == &other.a ? other.b : other.a);

	crossRoads.emplace_back();
	CrossRoad& ip_crossRoad = crossRoads.back();

	complementing_to_way.b.crossRoad = wayFurther.crossRoad;
	complementing_to_other.b.crossRoad = otherFurther.crossRoad;

	wayCloser.crossRoad = otherCloser.crossRoad =
	    complementing_to_way.a.crossRoad = complementing_to_other.a.crossRoad =
	        &ip_crossRoad;

	if (wayFurther.crossRoad == nullptr) {
		crossRoads.emplace_back();
		wayFurther.crossRoad = &crossRoads.back();
	}
	if (otherFurther.crossRoad == nullptr) {
		crossRoads.emplace_back();
		otherFurther.crossRoad = &crossRoads.back();
	}
}

void injectWay(WideRoads& ways, WideRoad2& way, CrossRoads& crossRoads)
{
	for (auto& other : ways) {
		if (&other == &way) continue;
		const std::pair<bool, Point2>& intersect_result = intersect(way, other);

		if (intersect_result.first) {
			const Point2& intersection_point = intersect_result.second;
			intersect(ways, way, other, intersection_point, crossRoads);
		}
	}
}

void normalizeWays(WideRoads& ways, CrossRoads& crossRoads)
{

	for (auto& i : ways) {
		injectWay(ways, i, crossRoads);
	}
}

std::pair<Segment2, Segment2> createWalls(const WideRoad2& way)
{
	double slope = way.segment().slope;

	float angle_sin = static_cast<float>(sin(slope)),
	      angle_cos = static_cast<float>(cos(slope)),
	      deltaXA = way.a.width * angle_sin, deltaXB = way.b.width * angle_sin,
	      deltaYA = way.a.width * angle_cos, deltaYB = way.b.width * angle_cos;

	const Point2 &a = way.a.point, &b = way.b.point;

	return {{{a.x + deltaXA, a.y - deltaYA}, {b.x + deltaXB, b.y - deltaYB}},
	        {{a.x - deltaXA, a.y + deltaYA}, {b.x - deltaXB, b.y + deltaYB}}};
}

void dump(const ColorSegmentList& maze)
{
	for (const ColorSegment2& segment : maze) {
		LOG << segment << ptr(segment.line.a) << " " << ptr(segment.line.b);
	}
}

ColorSegmentList createWalls(const WideRoads& input_paths,
                             const CrossRoads& crossRoads)
{
	ColorSegmentList generated_maze;
	for (const WideRoad2& path : input_paths) {
		auto walls = createWalls(path);
		generated_maze.push_back(toColorSegment(walls.first));
		generated_maze.push_back(toColorSegment(walls.second));
		generated_maze.push_back(toColorSegment(path));
	}

	for (const auto& crossRoad : crossRoads) {
		const auto& points = crossRoad.points;
		if (points.size() < 2) continue;
		LOG << "HERER";
		for (auto p = points.begin() + 1; p != points.end(); ++p) {
			generated_maze.push_back(toWhiteSegment2(Segment2{**p, **(p - 1)}));
		}
	}
	return generated_maze;
}

math::ColorSegmentList Builder::build_from_paths(WideRoads& input_paths)
{
	CrossRoads crossRoads;
	normalizeWays(input_paths, crossRoads);

	ColorSegmentList generated_maze = createWalls(input_paths, crossRoads);

	LOG("%lu walls generated from %lu lines", generated_maze.size(),
	    input_paths.size());

	dump(generated_maze);

	return generated_maze;
}
