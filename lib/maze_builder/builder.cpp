#include "builder.h"

#include "logger/include/logger.h"
#include "math/include/geometry_io.h"

#include <cassert>
#include <cmath>

#include <iostream>
#include <list>
#include <set>
#include <vector>

typedef std::vector<Color> Colors;
typedef std::list<CrossRoad> CrossRoads;
typedef std::list<std::pair<std::pair<Segment2, Segment2>, WideRoad2>> PWalls;

constexpr long double PI = 3.141592653589793238462643383279502884L;

constexpr bool FORCE_VALIDATE = false;

constexpr unsigned WIDEPOINT_WIDTH = 3;

Color getColor()
{
	static Colors colors = {
	    {1, 1, 0.1f, "L yellow"},    {0, 0.3f, 1.f, "D blue"},
	    {0.5f, 0, 0.8f, "violet"},   {1, 0.5f, 0, "orange"},
	    {.5f, .5f, .5f, "silver"},   {1.f, 0, 0, "red"},
	    {1.f, 0.5f, 0.5f, "pink"},   {0, .7f, 1.f, "L blue"},
	    {0.7f, 0.2f, 0.3f, "brown"}, {0.2f, 1, 0.2f, "L green"},
	    {0, 0.5f, 0, "D green"}};
	static auto color_count = colors.size(), color = 0lu;

	return colors[(color++) % color_count];
}

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

			// JUST_LOG << p.color << " " << get_ptr_val(p.a.crossRoad) << " "
			//<< get_ptr_val(p.b.crossRoad);
		}
		{
			auto& p = _p.first.second;
			count_unique.insert(p.a.crossRoad);
			count_unique.insert(p.b.crossRoad);

			// JUST_LOG << p.color << " " << get_ptr_val(p.a.crossRoad) << " "
			//<< get_ptr_val(p.b.crossRoad);
		}
	}
	// JUST_LOG << "Unique crossRoads: " << count_unique.size();
}

void validate_cross_road(CrossRoad* crossRoad)
{
	if (crossRoad == nullptr) {
		throw "CrossRoad pointer is empty!";
	}
	if (crossRoad->points.size() < 1) {
		throw "CrossRoads should have at least one point!";
	}
	// if (crossRoad->lines.size() < 2) {
	// throw "CrossRoads can't have less than 2 lines!";
	//}
}

void validate_walls(const PWalls& walls, bool force = FORCE_VALIDATE)
{
	if (!force) return;

	bool valid = true;
	for (const auto& _wall : walls) {
		auto& wall = _wall.first;
		if (wall.first.a.crossRoad != wall.second.a.crossRoad) {
			// JUST_LOG << "pointA's crossRoad is not the same as opposite's "
			//"A.crossRoad";
			valid = false;
		}
		if (wall.first.b.crossRoad != wall.second.b.crossRoad) {
			// JUST_LOG << "pointB's crossRoad is not the same as opposite's "
			//"B.crossRoad";
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

bool tryToInsert(CrossRoad& cr, const WidePoint2& point_to_insert)
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

bool point_inside_segment(Segment2 line, Point2 point)
{
	double x = point.x, x1 = line.a.x, x2 = line.b.x, y = point.y,
	       y1 = line.a.y, y2 = line.b.y;
	// http://stackoverflow.com/a/17590923
	double AB = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	double AP = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
	double PB = sqrt((x2 - x) * (x2 - x) + (y2 - y) * (y2 - y));
	double res = AP + PB - AB;
	double const deviation = 1e-6;
	LOG("%2.7f %s", res, fabs(res) < deviation ? "yes!" : "no");
	return fabs(res) < deviation;
}

void add_a_single_way_to_maze(bool join, PWalls& wallsP, const WideRoad2& way,
                              CrossRoads& cross_roads)
{
	const Segment2 line = way.getSegmnet2();
	double line_angle = atan((line.b.y - line.a.y) / (line.b.x - line.a.x));
	float angle_sin = static_cast<float>(sin(line_angle)),
	      angle_cos = static_cast<float>(cos(line_angle)),
	      deltaXA = way.a.width * angle_sin, deltaXB = way.b.width * angle_sin,
	      deltaYA = way.a.width * angle_cos, deltaYB = way.b.width * angle_cos;

	Segment2 _upper = {{line.a.x + deltaXA, line.a.y - deltaYA},
	                   {line.b.x + deltaXB, line.b.y - deltaYB},
	                   getColor()},
	         _lower = {{line.a.x - deltaXA, line.a.y + deltaYA},
	                   {line.b.x - deltaXB, line.b.y + deltaYB},
	                   getColor()};

	wallsP.push_back({{_upper, _lower}, way});
	Segment2& upper = wallsP.back().first.first;
	Segment2& lower = wallsP.back().first.second;
	WideRoad2& middle = wallsP.back().second;

	cross_roads.push_back({{way.a}});
	lower.a.crossRoad = upper.a.crossRoad = &cross_roads.back();

	cross_roads.push_back({{way.b}});
	lower.b.crossRoad = upper.b.crossRoad = &cross_roads.back();

	LOG(Logger::Green)("INSERT: %s/%s", upper.color.name, lower.color.name);

	++LOG;

	for (auto& _wall : wallsP) {
		if (!join) break;
		auto& wall = _wall.first;
		if (&wall.first == &upper) continue;

		LOG(Logger::White) << "CHECK WITH " << wall.first.color << "/"
		                   << wall.second.color << ":" << Logger::Default;
		Point2 ipUpper{}, ipLower{}, ipUpperOpposite{}, ipLowerOpposite{};
		const bool iupper = wall.first.intersectsWith(upper, &ipUpper),
		           ilower = wall.first.intersectsWith(lower, &ipLower),
		           ilowerOpposite =
		               wall.second.intersectsWith(lower, &ipLowerOpposite),
		           iupperOpposite =
		               wall.second.intersectsWith(upper, &ipUpperOpposite);

		// nothing intersecting here...
		if (!(iupper || ilower || iupperOpposite || ilowerOpposite)) {
			continue;
		}

		++LOG;

		// middle-middle intersection
		if (iupper && ilower && iupperOpposite && ilowerOpposite) {
			LOG("middle-middle");
			{
				CrossRoad croad{};
#pragma message("Calculate WidePoint2 width properly")
				croad.points.push_back(
				    {middleOf({ipLower, ipUpper}), WIDEPOINT_WIDTH});
				croad.points.push_back(
				    {middleOf({ipLowerOpposite, ipUpperOpposite}),
				     WIDEPOINT_WIDTH});
				croad.points.push_back(
				    {middleOf({ipLower, ipLowerOpposite}), WIDEPOINT_WIDTH});
				croad.points.push_back(
				    {middleOf({ipUpper, ipUpperOpposite}), WIDEPOINT_WIDTH});

				cross_roads.push_back(croad);
			}
			CrossRoad& croad = cross_roads.back();

			Segment2 new_lower = {
			    Segment2{ipLower, ipLowerOpposite}.getEndCloserTo(lower.b),
			    lower.b, getColor()};
			Segment2 new_upper = {
			    Segment2{ipUpper, ipUpperOpposite}.getEndCloserTo(upper.b),
			    upper.b, getColor()};

			new_upper.a.crossRoad = &croad;
			new_upper.b.crossRoad = upper.b.crossRoad;

			new_lower.a.crossRoad = &croad;
			new_lower.b.crossRoad = lower.b.crossRoad;

			// note - push front, we don't want them to be indexed again
			wallsP.push_back({{new_upper, new_lower}, {new_upper, new_lower}});

			// OTHER COMPLEMENTING WALLS
			Segment2 new_lower_other = {
			    Segment2{ipUpper, ipLower}.getEndCloserTo(wall.first.b),
			    wall.first.b, getColor()};
			Segment2 new_upper_other = {
			    Segment2{ipUpperOpposite, ipLowerOpposite}.getEndCloserTo(
			        wall.second.b),
			    wall.second.b, getColor()};

			new_upper_other.a.crossRoad = &croad;
			new_upper_other.b.crossRoad = wall.first.b.crossRoad;

			new_lower_other.a.crossRoad = &croad;
			new_lower_other.b.crossRoad = wall.second.b.crossRoad;

			wallsP.push_back({{new_upper_other, new_lower_other},
			                  {new_upper_other, new_lower_other}});

			// CUT THIS AND OTHER
			upper.b.moveTo(
			    Segment2{ipUpper, ipUpperOpposite}.getEndCloserTo(upper.a));
			lower.b.moveTo(
			    Segment2{ipLower, ipLowerOpposite}.getEndCloserTo(lower.a));
			middle.b.point.moveTo(middleOf(lower.b, upper.b));

			upper.b.crossRoad = &croad;
			lower.b.crossRoad = &croad;

			wall.first.b.moveTo(
			    Segment2{ipLower, ipUpper}.getEndCloserTo(wall.first.a));
			wall.second.b.moveTo(
			    Segment2{ipLowerOpposite, ipUpperOpposite}.getEndCloserTo(
			        wall.second.a));
			_wall.second.b.point.moveTo(middleOf(wall.first.b, wall.second.b));

			wall.first.b.crossRoad = &croad;
			wall.second.b.crossRoad = &croad;
		} else { // try to insert that point of lower/upper which is closer to
			     // the crossPoint to the crossRoad of that point of
			     // wall/opposite which is closer to the crossPoint. If yes, we
			     // have an end-to-end connection
			bool this_join_a = (&way.getEndCloserTo(wall.second) == &way.a),
			     other_join_a = (&_wall.second.getEndCloserTo(
			                         middle.getSegmnet2()) == &_wall.second.a);

			CrossRoad* other_closer_croad =
			    (other_join_a ? wall.first.a : wall.first.b).crossRoad;

			// LOG << way.getSegmnet2() << " " << static_cast<int>(this_join_a)
			//<< '\n';
			// LOG << _wall.second.getSegmnet2() << " "
			//<< static_cast<int>(other_join_a) << '\n';

			if (tryToInsert(*other_closer_croad, this_join_a ? way.a : way.b)) {
				LOG("end-end");

				if ((this_join_a ? upper.a : upper.b).crossRoad->points.size() <
				    2) {
					cross_roads.remove(
					    *(this_join_a ? upper.a : upper.b).crossRoad);
				}

				(this_join_a ? upper.a : upper.b).crossRoad =
				    (this_join_a ? lower.a : lower.b).crossRoad =
				        other_closer_croad;

				// Point2* common_point;
				// Segment2 *my_segm_to_cut, *other_segm_to_cut;

				if (!iupperOpposite && !ilowerOpposite) {
					if (iupper) {
						// cut at iupper
						// other_segm_to_cut = &wall.first;
						// my_segm_to_cut = &upper;
						// common_point = &ipUpper;
					} else if (ilower) {
						// cut at ilower
						// common_point = &ipLower;
						// my_segm_to_cut = &lower;
						// other_segm_to_cut = &wall.first;
					}
				} else if (!iupper && !ilower) {
					if (iupperOpposite && !ilowerOpposite) {
						// cut at iupperOpposite
						// my_segm_to_cut = &upper;
						// common_point = &ipUpperOpposite;
						// other_segm_to_cut = &wall.second;
					} else if (ilowerOpposite && !iupperOpposite) {
						// cut at ilowerOpposite
						lower.getEndCloserTo(ipLowerOpposite)
						    .moveTo(ipLowerOpposite);
						wall.second.getEndCloserTo(ipLowerOpposite)
						    .moveTo(ipLowerOpposite);
					} else {
						throw "This can't happen";
						// opposite crossed by both upper & lower,
						// not opposite not crossed by any of them
						const Point2& cross_point =
						    Segment2{ipLowerOpposite, ipUpperOpposite}
						        .getEndCloserTo(other_join_a ? wall.second.b
						                                     : wall.second.a);

						(other_join_a ? wall.second.a : wall.second.b)
						    .moveTo(cross_point);
					}
				} else {
					// const bool iu = iupper, il = ilower, iuo =
					// iupperOpposite,
					// ilo = ilowerOpposite;

					// JUST_LOG << iu << il << iuo << ilo;

					// throw "No idea.";
				}
			} else { // otherwise, we have a middle-to-end join
				const bool iu = iupper, il = ilower, iuo = iupperOpposite,
				           ilo = ilowerOpposite;

				LOG("Middle-end");

				bool this_end_joins_2_fixed_points_intersect =
				         (iu && il && !iuo && !ilo) ||
				         (iuo && ilo && !iu && !il),
				     other_end_joins_2_fixed_points_on_the_one_side =
				         (iu && iuo && !il && !ilo) ||
				         (il && ilo && !iu && !iuo),
				     only_one_point_is_not_part_of_intersection =
				         (!iu && iuo && il && ilo) ||
				         (iu && !iuo && il && ilo) ||
				         (iu && iuo && !il && ilo) || (iu && iuo && il && !ilo),
				     only_one_point_intersects = (1 == iu + il + iuo + ilo);

				bool this_joins_other =
				    this_end_joins_2_fixed_points_intersect ||
				    !other_end_joins_2_fixed_points_on_the_one_side;

				// JUST_LOG << this_end_joins_2_fixed_points_intersect <<
				// other_end_joins_2_fixed_points_on_the_one_side;

				if (only_one_point_is_not_part_of_intersection) {
					bool try_lower = false;
					Point2* point_to_check = nullptr;

					if (!iu) {
						point_to_check = &ipUpper;
					}
					if (!il) {
						try_lower = true;
						point_to_check = &ipLower;
					}
					if (!iuo) {
						// try_opposite = true;
						point_to_check = &ipUpperOpposite;
					}
					if (!ilo) {
						try_lower = true;
						// try_opposite = true;
						point_to_check = &ipLowerOpposite;
					}

					if (!point_inside_segment(try_lower ? lower : upper,
					                          *point_to_check)) {
						// this joins other
						this_joins_other = true;
					} else
						this_joins_other = false;
				}

				if (only_one_point_intersects) {
					if (iu) {
						if (!point_inside_segment(lower, ipLower)) {
							this_joins_other = true;
						}
					} else if (il) {
						if (!point_inside_segment(upper, ipUpper)) {
							this_joins_other = false;
						}
					} else if (iuo) {
						if (!point_inside_segment(lower, ipLowerOpposite)) {
							this_joins_other = true;
						}
					} else {
						LOG(Logger::Red)("unimplemented!");
						this_joins_other = true;
						// too lazy now
						// throw "Unimplemented.";
					}
				}

				// other end -> this middle
				if (!this_joins_other) {
					LOG("Other's end joins this's middle");
					bool other_cut_a =
					    (&wall.first.getEndCloserTo(ipUpper) == &wall.first.a);
					bool joins_from_upper = (iupper && iupperOpposite) ||
					                        ((iupper || iupperOpposite) &&
					                         (!ilower || !ilowerOpposite));

					Segment2
					    new_upper =
					        {Segment2{ipUpper, ipUpperOpposite}.getEndCloserTo(
					             joins_from_upper ? upper.b : upper.a),
					         upper.b, getColor()},
					    new_lower = {
					        Segment2{ipLower, ipLowerOpposite}.getEndCloserTo(
					            joins_from_upper ? lower.a : lower.b),
					        lower.b, getColor()};

					wallsP.push_back(
					    {{new_upper, new_lower}, {new_upper, new_lower}});

					wallsP.back().first.first.a.crossRoad =
					    other_cut_a ? wall.first.a.crossRoad
					                : wall.first.b.crossRoad;
					wallsP.back().first.second.a.crossRoad =
					    other_cut_a ? wall.first.a.crossRoad
					                : wall.first.b.crossRoad;

					upper.b.moveTo(
					    Segment2{ipUpper, ipUpperOpposite}.getEndCloserTo(
					        upper.a));
					lower.b.moveTo(
					    Segment2{ipLower, ipLowerOpposite}.getEndCloserTo(
					        lower.a));

					lower.b.crossRoad = upper.b.crossRoad =
					    (other_cut_a ? wall.first.a : wall.first.b).crossRoad;

					(other_cut_a ? wall.first.a : wall.first.b)
					    .moveTo(joins_from_upper ? ipUpper : ipLower);
					(other_cut_a ? wall.second.a : wall.second.b)
					    .moveTo(joins_from_upper ? ipUpperOpposite
					                             : ipLowerOpposite);

					(other_cut_a ? _wall.second.a : _wall.second.b)
					    .point.moveTo(
					        other_cut_a
					            ? middleOf(wall.first.a, wall.second.a)
					            : middleOf(wall.first.b, wall.second.b));

					middle.b.point.moveTo(
					    joins_from_upper
					        ? middleOf(ipUpperOpposite, ipLowerOpposite)
					        : middleOf(ipUpper, ipLower));
				} else {
					// end of this joining middle of other
					LOG("This end joins other's middle");
					bool this_cut_a =
					    (&upper.getEndCloserTo(ipUpper) == &upper.a);
					bool joins_from_opposite =
					    (iupperOpposite && ilowerOpposite) ||
					    ((iupperOpposite || ilowerOpposite) &&
					     (!iupper || !ilower));

					Segment2
					    new_first = {Segment2{ipUpper, ipLower}.getEndCloserTo(
					                     joins_from_opposite ? wall.first.a
					                                         : wall.first.b),
					                 wall.first.b, getColor()},
					    new_second = {Segment2{ipUpperOpposite, ipLowerOpposite}
					                      .getEndCloserTo(joins_from_opposite
					                                          ? wall.second.b
					                                          : wall.second.a),
					                  wall.second.b, getColor()};

					wallsP.push_back(
					    {{new_first, new_second}, {new_first, new_second}});
					wallsP.back().first.first.a.crossRoad =
					    this_cut_a ? upper.a.crossRoad : upper.b.crossRoad;
					wallsP.back().first.second.a.crossRoad =
					    this_cut_a ? lower.a.crossRoad : lower.b.crossRoad;

					wall.first.b
					    .moveTo(Segment2{ipUpper, ipLower}.getEndCloserTo(
					        wall.first.a))
					    .crossRoad =
					    (this_join_a ? upper.a : upper.b).crossRoad;
					wall.second.b
					    .moveTo(Segment2{ipUpperOpposite, ipLowerOpposite}
					                .getEndCloserTo(wall.second.a))
					    .crossRoad = wall.first.b.crossRoad;

					(this_cut_a ? upper.a : upper.b)
					    .moveTo(joins_from_opposite ? ipUpperOpposite
					                                : ipUpper);

					(this_cut_a ? lower.a : lower.b)
					    .moveTo(joins_from_opposite ? ipLowerOpposite
					                                : ipLower);

					(this_cut_a ? middle.a : middle.b)
					    .point.moveTo(this_cut_a ? middleOf(upper.a, lower.a)
					                             : middleOf(upper.b, lower.b));

					_wall.second.b.point.moveTo(
					    joins_from_opposite
					        ? middleOf(ipUpper, ipUpperOpposite)
					        : middleOf(ipLower, ipLowerOpposite));
				}
			}
		}
		--LOG;
	}

	--LOG;

	FORCE_VALIDATE&& LOG("Will validate.");
	validate_walls(wallsP);
}

void Builder::build_from_paths(const Ways& paths, std::list<Segment2>& maze)
{
	// loguru::g_stderr_verbosity = 3;
	// loguru::g_colorlogtostderr = true;

	CrossRoads cross_roads;

	PWalls wallsP;

	// add_a_single_way_to_maze(wallsP, *++paths.begin(), cross_roads);
	// add_a_single_way_to_maze(wallsP, *paths.begin(), cross_roads);

	unsigned way_count = 0;
	for (const auto& way : paths) {
		if (way.a.width != WIDEPOINT_WIDTH || way.b.width != WIDEPOINT_WIDTH) {
			LOG << way.a.point << " - " << way.b.point
			    << "; a.width: " << way.a.width << ", b.width: " << way.b.width;
			throw "WidePoint's width must be 3(or another fixed value "
			      "specified by WIDEPOINT_WIDTH) until other problems are "
			      "resolved";
		}
		add_a_single_way_to_maze(join, wallsP, way, cross_roads);
		if (++way_count == max_count) break;
	}

	for (auto i : wallsP) {
		maze.push_back(i.first.first);
		maze.push_back(i.first.second);
		Segment2 middle = i.second.getSegmnet2();
		middle.color = getColor();
		maze.push_back(middle);
	}

	LOG("Ways added to maze. Will validate.");

	validate_walls(wallsP, true);

	LOG("%lu walls generated from %lu lines\n", maze.size(), paths.size());

	dump(wallsP);
	LOG("CrossRoads in std::list cross_roads: %lu\n", cross_roads.size());
}
