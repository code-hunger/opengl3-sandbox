#include "builder.h"

#include "logger/include/logger.h"
#include "math/include/geometry_io.h"

#include <cassert>
#include <cmath>

#include <list>
#include <set>

constexpr long double PI = 3.141592653589793238462643383279502884L;

constexpr bool FORCE_VALIDATE = false;

constexpr unsigned WIDEPOINT_WIDTH = 3;

inline unsigned long get_ptr_val(void* ptr)
{
	return reinterpret_cast<unsigned long>(ptr) % 10000l;
}

// It's wrong. To be removed
// @Deprecated
void dump(PWalls walls)
{
	std::set<CrossRoad*> count_unique;
	for (const auto& _p : walls) {
		{
			auto& p = _p.first.first;
			count_unique.insert(p.a.crossRoad);
			count_unique.insert(p.b.crossRoad);

			LOG << p.color << " " << get_ptr_val(p.a.crossRoad) << " "
			    << get_ptr_val(p.b.crossRoad);
		}
		{
			auto& p = _p.first.second;
			count_unique.insert(p.a.crossRoad);
			count_unique.insert(p.b.crossRoad);

			LOG << p.color << " " << get_ptr_val(p.a.crossRoad) << " "
			    << get_ptr_val(p.b.crossRoad);
		}
	}
	LOG << "Unique crossRoads: " << count_unique.size();
}

void validate_cross_road(CrossRoad* crossRoad)
{
	if (crossRoad == nullptr) {
		throw "CrossRoad pointer is empty!";
	}
	if (crossRoad->points.size() < 1) {
		throw "CrossRoads should have at least one point!";
	}
}

void validate_walls(const PWalls& walls, bool force = FORCE_VALIDATE)
{
	if (not force) return;

	bool valid = true;
	for (const auto& _wall : walls) {
		auto& wall = _wall.first;
		if (wall.first.a.crossRoad != wall.second.a.crossRoad) {
			LOG << "pointA's crossRoad is not the same as opposite's "
			       "A.crossRoad";
			valid = false;
		}
		if (wall.first.b.crossRoad != wall.second.b.crossRoad) {
			LOG << "pointB's crossRoad is not the same as opposite's "
			       "B.crossRoad";
			valid = false;
		}

		try {
			// first or second - doesn't matter, they're equal
			validate_cross_road(wall.first.a.crossRoad);
			validate_cross_road(wall.first.b.crossRoad);
		} catch (const char* err) {
			LOG << err;
			valid = false;
		}
	}
	if (!valid) {
		throw "Walls' state invalid!";
	}
}

bool tryToInsert(CrossRoad& cr, WidePoint2 const& point_to_insert)
{
	if (cr.points.size() < 1)
		throw "CrossRoad MUST have at least 1 point inside!";

	for (const auto& point : cr.points) {
		// Note that they're always positive
		float dist = calcSquaredLen(point.point, point_to_insert.point),
		      max_allowed = point.width + point_to_insert.width;

		// dist is not squared, so we do max_allowed ^ 2
		if (dist < max_allowed * max_allowed) {
			cr.points.push_back(point_to_insert);
			return true;
		}
	}
	return false;
}

std::pair<Segment2, Segment2> Builder::createWalls(WideRoad2 const& way)
{
	const Segment2 line = way.getSegmnet2();
	double line_angle = atan((line.b.y - line.a.y) / (line.b.x - line.a.x));
	float angle_sin = static_cast<float>(sin(line_angle)),
	      angle_cos = static_cast<float>(cos(line_angle)),
	      deltaXA = way.a.width * angle_sin, deltaXB = way.b.width * angle_sin,
	      deltaYA = way.a.width * angle_cos, deltaYB = way.b.width * angle_cos;

	return {{{line.a.x + deltaXA, line.a.y - deltaYA},
	         {line.b.x + deltaXB, line.b.y - deltaYB},
	         Color::next()},
	        {{line.a.x - deltaXA, line.a.y + deltaYA},
	         {line.b.x - deltaXB, line.b.y + deltaYB},
	         Color::next()}};
}

void normalizeWay(Ways& paths, Ways::value_type& way)
{
	for (auto& other : paths) {
		auto intersects = way.getSegmnet2().intersectsWith(other.getSegmnet2());
	}
}

void Builder::normalizeWays(Ways& paths)
{
	for (auto& i : paths) {
		normalizeWay(paths, i);
	}
}

std::list<Segment2> Builder::build_from_paths(Ways& paths)
{
	CrossRoads cross_roads;

	normalizeWays(paths);

	std::list<Segment2> maze;

	for (auto i : paths) {
		Segment2 middle = i.getSegmnet2();
		middle.color = Color::next();
		maze.push_back(middle);
	}

	LOG("Ways added to maze. Will validate.");

	// validate_walls(wallsP, true);

	LOG("%lu walls generated from %lu lines", maze.size(), paths.size());

	// dump(wallsP);
	LOG("CrossRoads in std::list cross_roads: %lu", cross_roads.size());
	return maze;
}
