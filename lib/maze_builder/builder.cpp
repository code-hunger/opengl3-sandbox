#include "builder.h"

#include "logger/include/logger.h"
#include "math/include/geometry.h"
#include "math/include/types_io.h"

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
	if (way.len2() > minLength * minLength) {
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
                                 WideRoad2& way, WideRoad2& inserted_complement)
{
	LOG << "Insert complement" << inserted_complement;
	closer.point = ip;
	closer.width = ipoint_width;

	inserted_complement.b.crossRoad = closer.crossRoad;

	{
		// remove closer from its crossroad since we're moving (we *will* move
		// it 3 lines later) closer to ip
		CrossRoad::Points& points = (*inserted_complement.b.crossRoad)->points;
		auto search_predicate =
		    [&closer](CrossRoad::Points::value_type const& val) {
			    return val.first == &closer;
			};
		auto it = std::find_if(points.begin(), points.end(), search_predicate);
		if (it == points.end()) throw "A very very bad logic error";
		points.erase(it);
		points.emplace_back(&inserted_complement.b, &inserted_complement);
	}

	closer.crossRoad = inserted_complement.a.crossRoad = ip_crossRoad;

	{
		auto& points = ip_crossRoad->points;
		points.emplace_back(&closer, &way);
		points.emplace_back(&inserted_complement.a, &inserted_complement);
	}
}

void combineCroads(CrossRoads& crossRoads, CrossRoads::iterator& destination,
                   WidePoint2& source_point)
{
	if (!source_point.crossRoad) throw "No crossRoad in point";

	if (source_point.crossRoad == destination) {
		WARN("Source same as destination in %s on line %d", __FILE__, __LINE__);
		return;
	}

	LOG << "Merging " << ptr(**source_point.crossRoad) << " into "
	    << ptr(*destination);

	CrossRoads::iterator source_iterator = *source_point.crossRoad;

	++Logger::get();

	for (auto const& i : source_iterator->points) {
		LOG << "Moving croad from " << *i.first;
		i.first->crossRoad = destination;
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
	WidePoint2& wayCloser = way.getEndCloserTo(ip);

	const float ip_width_way = way.widthAt(ip);

	WideRoad2 complementing_to_way = {{ip, ip_width_way}, wayCloser},
	          *inserted_complement_way =
	              insertIfBipgEnough(ways, complementing_to_way,
	                                 std::max(way.a.width, way.b.width));

	if (inserted_complement_way) {
		insert_croad_for_complement(wayCloser, ip, ip_width_way, ip_crossRoad,
		                            way, *inserted_complement_way);
	} else {
		combineCroads(crossRoads, ip_crossRoad, wayCloser);
	}
}

void intersect(WideRoads& ways, WideRoad2& way, WideRoad2& other,
               CrossRoads& crossRoads)
{
	const auto& intersect_result = intersect(way, other);

	LOG << "Intersect with " << other << " on " << intersect_result.point
	    << " - " << intersect_result.intersects;

	Point2 const& ip = intersect_result.point;

	if (intersect_result.intersects) {
		CrossRoads::iterator const& ip_crossRoad = create_crossRoad(crossRoads);

		intersect(ways, way, ip_crossRoad, ip, crossRoads);
		intersect(ways, other, ip_crossRoad, ip, crossRoads);
	}

	else if (distanceToLine(way, other) < 5) {
		LOG << "But... is close!";

		if (way.segment().isInside(ip)) {
			CrossRoads::iterator ip_crossRoad = create_crossRoad(crossRoads);
			intersect(ways, way, ip_crossRoad, ip, crossRoads);
			combineCroads(crossRoads, ip_crossRoad, other.getEndCloserTo(ip));
		} else if (other.segment().isInside(ip)) {
			CrossRoads::iterator ip_crossRoad = create_crossRoad(crossRoads);
			intersect(ways, other, ip_crossRoad, ip, crossRoads);
			combineCroads(crossRoads, ip_crossRoad, way.getEndCloserTo(ip));
		} else {
			LOG << "Edges close, combine them";
			combineCroads(crossRoads, *way.getEndCloserTo(ip).crossRoad,
			              other.getEndCloserTo(ip));
		}
	}
}

void addCrossRoadsIfNone(WidePoint2& p, WideRoad2& way, CrossRoads& crossRoads)
{
	if (!p.crossRoad) {
		p.crossRoad = create_crossRoad(crossRoads);
		(*p.crossRoad)->points.emplace_back(&p, &way);
	}
}

bool find_points(const CrossRoad::Points& haystack, const WidePoint2& needle)
{
	return std::find_if(haystack.begin(), haystack.end(),
	                    [&needle](CrossRoad::Points::value_type const& val) {
		                    return val.first == &needle;
		                }) != haystack.end();
}

void normalizeWays(WideRoads& ways, CrossRoads& crossRoads)
{
	// explicitly calc ways.end() everytime  since intersect() does push_back()
	// on newly added ways.  Those ways need to be injected and checked, too
	for (auto way = ways.begin(); way != ways.end(); ++way) {
		addCrossRoadsIfNone(way->a, *way, crossRoads);
		addCrossRoadsIfNone(way->b, *way, crossRoads);

		LOG << "Will inject " << *way << "; croad: " << ptr(**way->a.crossRoad)
		    << " " << ptr(**way->b.crossRoad);
		for (auto other = ways.begin(); other != way; ++other) {
			const auto &a_points = (*way->a.crossRoad)->points,
			           &b_points = (*way->b.crossRoad)->points;

			if (find_points(a_points, other->a) ||
			    find_points(b_points, other->b) ||
			    find_points(a_points, other->b) ||
			    find_points(b_points, other->a)) {
				continue;
			}
			INDENT(intersect(ways, *way, *other, crossRoads));
		}
	}
}

std::pair<Segment2, Segment2> createWallsPair(const WideRoad2& way)
{
	float slope = way.segment().slope();

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

#define toDarkSegment(t) t.colorSegment({.1f, .1f, .1f, "dark"})

#include <map>

// For each cross road holds pairs of the walls ending on it
typedef std::map<CrossRoad*,
                 std::vector<std::tuple<const WideRoad2*, bool,
                                        std::pair<Segment2*, Segment2*>, bool>>>
    CrossRoadWallPairs;

auto createWalls(CrossRoadWallPairs& crossRoadWallPairs, WideRoads& ways)
{
	std::list<Segment2> generated_maze;
	for (const WideRoad2& way : ways) {
		const auto& walls = createWallsPair(way);
		generated_maze.push_back(walls.first);
		auto& just_pushed_a = generated_maze.back();
		generated_maze.push_back(walls.second);
		auto& just_pushed_b = generated_maze.back();

		typedef CrossRoadWallPairs::value_type::second_type::value_type
		    This_pair;
		std::tuple_element<2, This_pair>::type this_pair_tw =
		    std::make_pair(&just_pushed_a, &just_pushed_b);
		This_pair this_pair_a = std::make_tuple(&way, 0, this_pair_tw, false);
		This_pair this_pair_b = std::make_tuple(&way, 1, this_pair_tw, false);

		crossRoadWallPairs[&**way.a.crossRoad].emplace_back(this_pair_a);
		crossRoadWallPairs[&**way.b.crossRoad].emplace_back(this_pair_b);
	}

	return generated_maze;
}

void dumpCroads(CrossRoads const& crossRoads, ColorSegmentList& generated_maze)
{
	LOG << crossRoads.size() << " croads!";
	for (const CrossRoad& crossRoad : crossRoads) {
		const CrossRoad::Points& points = crossRoad.points;

		LOG << "Croad " << ptr(crossRoad) << " has " << points.size()
		    << " points!";

		if (points.empty()) continue;

		for (auto& i : points) {
			LOG << *i.first;
		}
		for (auto p = points.cbegin(), q = p + 1; q != points.cend();
		     ++p, ++q) {
			Point2 p_p = p->first->point, p_q = q->first->point;
			p_p.x -= 5;
			p_q.x += 5;
			generated_maze.push_back(Segment2{p_p, p_q}.whiteSegment());
		}
	}
}

void sortCroadPoints(CrossRoadWallPairs::value_type& walls)
{
	CrossRoadWallPairs::value_type::second_type& points = walls.second;
	using points_elem_t =
	    std::remove_reference<decltype(points)>::type::value_type;

	// sort the walls at this joint by slope
	std::sort(points.begin(), points.end(),
	          [](points_elem_t& a, points_elem_t& b) {
		          const Segment2 &segment_a = std::get<0>(a)->segment(),
		                         &segment_b = std::get<0>(b)->segment();
		          double slope_a, slope_b;
		          if (std::get<1>(a) == 0)
			          slope_a = segment_a.slope();
		          else {
			          slope_a = segment_a.slopeB();
			          if (!std::get<3>(a)) {
				          auto& x = std::get<2>(a);
				          std::swap(x.first, x.second);
				          LOG << "Swap A";
						  std::get<3>(a) = true;
			          }
		          }

		          if (std::get<1>(b) == 0)
			          slope_b = segment_b.slope();
		          else {
			          slope_b = segment_b.slopeB();
			          if (!std::get<3>(b)) {
				          auto& x = std::get<2>(b);
				          std::swap(x.first, x.second);
				          LOG << "Swap B";
						  std::get<3>(b) = true;
			          }
		          }

		          if (slope_a < slope_b) return true;
		          if (slope_a > slope_b) return false;
		          return false;
		          // if (a.first->point.x > b.first->point.x) return true;
		          // if (a.first->point.x < b.first->point.x) return false;
		          // return a.first->point.y > b.first->point.y;
		      });

	LOG << "points count:" << points.size() << "\n";
}

#include "math/include/types_io.h"

ColorSegmentList Builder::build_from_paths(WideRoads& ways)
{
	++Logger::get();
	CrossRoads crossRoads;
	LOG << "Normalize ways - first " << ways.size();
	INDENT(normalizeWays(ways, crossRoads));
	LOG << "Normalized - " << ways.size();

	CrossRoadWallPairs crossRoadWallPairs;

	auto generated_maze = createWalls(crossRoadWallPairs, ways);

	LOG << "Croad wall pairs count: " << crossRoadWallPairs.size();
	for (auto& i : crossRoadWallPairs) {
		LOG << "New croad, sorting it.";
		sortCroadPoints(i);
		auto lines = i.second;
		for (auto i = 0ul, count = lines.size(); i < count - 1; ++i) {
			auto current = std::get<2>(lines[i]),
			     next = std::get<2>(lines[i + 1]);
			//
			LOG << "Current: " << *current.first << " | " << *current.second
			    << " is " << (std::get<1>(lines[i]) == 0 ? "a" : "b");
			LOG << "   Next: " << *next.first << " | " << *next.second << " is "
			    << (std::get<1>(lines[i + 1]) == 0 ? "a" : "b");

			Point2 *p1, *p2;
			if (std::get<1>(lines[i]) == 0)
				p1 = &current.second->a;
			else
				p1 = &current.second->b;
			if (std::get<1>(lines[i + 1]) == 0)
				p2 = &next.first->a;
			else
				p2 = &next.first->b;

			*p1 = *p2 = intersect(*current.second, *next.first).point;
			// break;
		}
	}

	// for (auto& i : crossRoadWallPairs) {
	//}

	LOG("%lu walls generated from %lu lines\n", generated_maze.size(),
	    ways.size());

	ColorSegmentList gen_maze;
	for (auto i : generated_maze) {
		gen_maze.emplace_back(i.colorSegment());
	}

	// dump(generated_maze);

	--Logger::get();
	return gen_maze;
}
