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

WideRoad2* insertIfBipgEnough(WideRoads& ways, const WideRoad2& way,
                              float minLength)
{
	if (calcSquaredLen(way) > minLength * minLength) {
		ways.push_back(way);
		return &ways.back();
	}
	return nullptr;
}

void insert_croad_for_complement(WidePoint2& closer, WidePoint2& farther,
                                 const Point2& ip, float ipoint_width,
                                 CrossRoads& crossRoads,
                                 CrossRoads::iterator ip_crossRoad,
                                 WideRoad2& inserted_complement)
{
	closer.point = ip;
	closer.width = ipoint_width;

	inserted_complement.b.crossRoad = closer.crossRoad;

	closer.crossRoad = inserted_complement.a.crossRoad = ip_crossRoad;

	if (!farther.crossRoad) {
		crossRoads.emplace_back();
		farther.crossRoad = std::prev(crossRoads.end());
	}
}

using std::experimental::optional;

void combineCroads(CrossRoads& crossRoads, CrossRoads::iterator& destination,
                   WidePoint2& source_point)
{
	if (source_point.crossRoad) {
		CrossRoads::iterator source_iterator = *source_point.crossRoad;
		std::vector<WidePoint2 *> &source_points = source_iterator->points,
		                          &dest_p = destination->points;

		for (WidePoint2* i : source_points) {
			i->crossRoad = destination;
		}

		dest_p.insert(dest_p.end(), source_points.begin(), source_points.end());

		crossRoads.erase(source_iterator);
	}
	source_point.crossRoad = destination;
}

void intersect(WideRoads& ways, WideRoad2& way, WideRoad2& other,
               const Point2& ip, CrossRoads& crossRoads)
{
	WidePoint2 &wayCloser = getEndCloserTo(way, ip),
	           &otherCloser = getEndCloserTo(other, ip),
	           &wayFarther = (&wayCloser == &way.a ? way.b : way.a),
	           &otherFarther = (&otherCloser == &other.a ? other.b : other.a);

	const float ip_width_way = get_width_at_point(way, ip),
	            ip_width_other = get_width_at_point(other, ip);

	WideRoad2 complementing_to_way = {{ip, ip_width_way}, wayCloser},
	          complementing_to_other = {{ip, ip_width_other}, otherCloser};

	WideRoad2 *inserted_complement_way =
	              insertIfBipgEnough(ways, complementing_to_way,
	                                 std::max(way.a.width, way.b.width)),
	          *inserted_complement_other =
	              insertIfBipgEnough(ways, complementing_to_other,
	                                 std::max(other.a.width, way.b.width));
	crossRoads.emplace_back();
	CrossRoads::iterator ip_crossRoad = std::prev(crossRoads.end());

	ip_crossRoad->points.push_back(&otherCloser);
	ip_crossRoad->points.push_back(&wayCloser);

	if (inserted_complement_way) {
		insert_croad_for_complement(wayCloser, wayFarther, ip, ip_width_way,
		                            crossRoads, ip_crossRoad,
		                            *inserted_complement_way);
	} else {
		combineCroads(crossRoads, ip_crossRoad, wayCloser);
	}
	if (inserted_complement_other) {
		insert_croad_for_complement(otherCloser, otherFarther, ip,
		                            ip_width_other, crossRoads, ip_crossRoad,
		                            *inserted_complement_other);
	} else {
		combineCroads(crossRoads, ip_crossRoad, otherCloser);
	}
}

void injectWay(WideRoads& ways, const WideRoads::iterator& way,
               CrossRoads& crossRoads)
{
	for (auto other = ways.begin(); other != way; ++other) {
		const auto& intersect_result = intersect(*way, *other);

		if (intersect_result.intersects) {
			const Point2& intersection_point = intersect_result.point;
			intersect(ways, *way, *other, intersection_point, crossRoads);
		}
	}
}

void normalizeWays(WideRoads& ways, CrossRoads& crossRoads)
{
	// explicitly calc ways.end() since injectWay push_back()s newly added ways.
	// Those ways need to be injected and checked, too
	for (auto way = ways.begin(); way != ways.end(); ++way) {
		injectWay(ways, way, crossRoads);
	}
}

std::pair<Segment2, Segment2> createWalls(const WideRoad2& way)
{
	float slope = way.segment().slope;

	float angle_sin = sinf(slope), angle_cos = cosf(slope),
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

ColorSegmentList createWalls(const WideRoads& ways,
                             const CrossRoads& crossRoads)
{
	ColorSegmentList generated_maze;
	for (const WideRoad2& way : ways) {
		//const auto& walls = createWalls(way);
		//generated_maze.push_back(toColorSegment(walls.first));
		//generated_maze.push_back(toColorSegment(walls.second));
		generated_maze.push_back(toColorSegment(way));
	}

	for (const CrossRoad& crossRoad : crossRoads) {
		const std::vector<WidePoint2*>& points = crossRoad.points;
		if (points.empty()) continue;

		for (auto p = points.begin(), q = p + 1; q != points.end(); ++p, ++q) {
			Point2 p_p = (*p)->point, p_q = (*q)->point;
			p_p.x -= 5;
			p_q.x += 5;
			generated_maze.push_back(toWhiteSegment2({p_p, p_q}));
		}
	}
	return generated_maze;
}

math::ColorSegmentList Builder::build_from_paths(WideRoads& ways)
{
	CrossRoads crossRoads;
	normalizeWays(ways, crossRoads);

	ColorSegmentList generated_maze = createWalls(ways, crossRoads);

	//LOG("%lu walls generated from %lu lines", generated_maze.size(),
		//ways.size());

	//dump(generated_maze);

	return generated_maze;
}
