#include "builder.h"

#include "logger/include/logger.h"
#include "math/include/geometry.h"
#include "math/include/geometry_io.h"

#include <algorithm>
#include <cassert>
#include <cmath>

#include <list>

#define INDENT(a)                                                              \
	++Logger::get();                                                           \
	a;                                                                         \
	--Logger::get();

constexpr long double PI = 3.141592653589793238462643383279502884L;

constexpr bool FORCE_VALIDATE = false;

constexpr unsigned WIDEPOINT_WIDTH = 3;

using namespace math;

template <typename T> unsigned long ptr(const T& a)
{
	return reinterpret_cast<unsigned long>(&a) % 10000;
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

CrossRoads::iterator create_crossRoad(CrossRoads& crossRoads)
{
	crossRoads.emplace_back();
	return --crossRoads.end();
}

void insert_croad_for_complement(WidePoint2& closer, const Point2& ip,
                                 float ipoint_width,
                                 CrossRoads::iterator ip_crossRoad,
                                 WideRoad2& inserted_complement)
{
	closer.point = ip;
	closer.width = ipoint_width;

	inserted_complement.b.crossRoad = closer.crossRoad;

	{
		// remove closer from its crossroad since we're moving (we *will* move
		// it 3 lines later) closer to ip
		auto& points = (*inserted_complement.b.crossRoad)->points;
		auto it = std::find(points.begin(), points.end(), &closer);
		if (it == points.end()) throw "A very very bad logic error";
		points.erase(it);
		points.push_back(&inserted_complement.b);
	}

	closer.crossRoad = inserted_complement.a.crossRoad = ip_crossRoad;

	{
		auto& points = ip_crossRoad->points;
		points.push_back(&closer);
		points.push_back(&inserted_complement.a);
	}
}

void combineCroads(CrossRoads& crossRoads, CrossRoads::iterator& destination,
                   WidePoint2& source_point)
{
	if (!source_point.crossRoad) throw "No crossRoad in point";

	if (source_point.crossRoad == destination) return;

	LOG << "Merging " << ptr(**source_point.crossRoad) << " into "
	    << ptr(*destination);

	CrossRoads::iterator source_iterator = *source_point.crossRoad;

	++Logger::get();

	for (WidePoint2* i : source_iterator->points) {
		LOG << "Moving croad from " << *i;
		i->crossRoad = destination;
		destination->points.emplace_back(i);
	}

	--Logger::get();

	LOG << "Destination has " << destination->points.size() << " points now";
	LOG << "Erasing " << ptr(*source_iterator);

	crossRoads.erase(source_iterator);

	source_point.crossRoad = destination;
}

void intersect(WideRoads& ways, WideRoad2& way,
               CrossRoads::iterator ip_crossRoad, const Point2& ip,
               CrossRoads& crossRoads)
{
	WidePoint2& wayCloser = getEndCloserTo(way, ip);

	const float ip_width_way = get_width_at_point(way, ip);

	WideRoad2 complementing_to_way = {{ip, ip_width_way}, wayCloser};

	WideRoad2* inserted_complement_way = insertIfBipgEnough(
	    ways, complementing_to_way, std::max(way.a.width, way.b.width));

	if (inserted_complement_way) {
		insert_croad_for_complement(wayCloser, ip, ip_width_way, ip_crossRoad,
		                            *inserted_complement_way);
	} else {
		combineCroads(crossRoads, ip_crossRoad, wayCloser);
	}
}

void injectWay(WideRoads& ways, const WideRoads::iterator& way,
               CrossRoads& crossRoads)
{
	for (auto other = ways.begin(); other != way; ++other) {
		const auto& intersect_result = intersect(*way, *other);

		LOG << "Intersect with " << *other << " - "
		    << intersect_result.intersects;
		if (intersect_result.intersects) {
			CrossRoads::iterator ip_crossRoad = create_crossRoad(crossRoads);
			INDENT(intersect(ways, *way, ip_crossRoad, intersect_result.point,
			                 crossRoads));
			INDENT(intersect(ways, *other, ip_crossRoad, intersect_result.point,
			                 crossRoads));
		}
	}
}

void addCrossRoadsIfNone(WidePoint2& p, CrossRoads& crossRoads)
{
	if (!p.crossRoad) {
		p.crossRoad = create_crossRoad(crossRoads);
		(*p.crossRoad)->points.push_back(&p);
	}
}

void addCrossRoadsIfNone(WideRoad2& way, CrossRoads& crossRoads)
{
	addCrossRoadsIfNone(way.a, crossRoads);
	addCrossRoadsIfNone(way.b, crossRoads);
}

void normalizeWays(WideRoads& ways, CrossRoads& crossRoads)
{
	// explicitly calc ways.end() everytime  since injectWay does push_back() on
	// newly added ways.  Those ways need to be injected and checked, too
	for (auto way = ways.begin(); way != ways.end(); ++way) {
		addCrossRoadsIfNone(*way, crossRoads);
		LOG << "Will inject" << *way;
		INDENT(injectWay(ways, way, crossRoads));
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

#define toDarkSegment(t) toColorSegment(t, {.1f, .1f, .1f, "dark"})

ColorSegmentList createWalls(const WideRoads& ways,
                             const CrossRoads& crossRoads)
{
	ColorSegmentList generated_maze;
	for (const WideRoad2& way : ways) {
		const auto& walls = createWalls(way);
		generated_maze.push_back(toDarkSegment(walls.first));
		generated_maze.push_back(toDarkSegment(walls.second));
		generated_maze.push_back(toColorSegment(way));
	}

	LOG << crossRoads.size() << " croads!";
	for (const CrossRoad& crossRoad : crossRoads) {
		const std::vector<WidePoint2*>& points = crossRoad.points;

		LOG << "Croad " << ptr(crossRoad) << " has " << points.size()
		    << " points!";

		if (points.empty()) continue;

		for (auto& i : points) {
			LOG << *i;
		}
		for (auto p = points.cbegin(), q = p + 1; q != points.cend();
		     ++p, ++q) {
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
	++Logger::get();
	CrossRoads crossRoads;
	LOG << "Normalize ways";
	INDENT(normalizeWays(ways, crossRoads));

	ColorSegmentList generated_maze = createWalls(ways, crossRoads);

	LOG("%lu walls generated from %lu lines\n", generated_maze.size(),
	    ways.size());

	// dump(generated_maze);

	--Logger::get();
	return generated_maze;
}
