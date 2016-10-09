#include "graphics/geometry_io.h"
#define BOOST_LOG_DYN_LINK

#include "maze_builder/builder.h"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"

#define JUST_LOG LOG_S(INFO)

#include <cassert>
#include <cmath>
const long double PI = 3.141592653589793238462643383279502884L;

#include <list>
#include <set>
#include <unordered_set>
#include <vector>

#include <iostream>
using std::cout;
using std::endl;

typedef std::vector<Color> Colors;
typedef std::list<CrossRoad> CrossRoads;
typedef std::list<std::pair<Segment2, Segment2>> PWalls;

constexpr bool FORCE_VALIDATE = 0;

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
			auto& p = _p.first;
			count_unique.insert(p.a.crossRoad);
			count_unique.insert(p.b.crossRoad);

			JUST_LOG << p.color << " " << get_ptr_val(p.a.crossRoad) << " "
			         << get_ptr_val(p.b.crossRoad);
		}
		{
			auto& p = _p.second;
			count_unique.insert(p.a.crossRoad);
			count_unique.insert(p.b.crossRoad);

			JUST_LOG << p.color << " " << get_ptr_val(p.a.crossRoad) << " "
			         << get_ptr_val(p.b.crossRoad);
		}
	}
	JUST_LOG << "Unique crossRoads: " << count_unique.size();
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
	for (const auto& wall : walls) {
		if (wall.first.a.crossRoad != wall.second.a.crossRoad) {
			JUST_LOG << "pointA's crossRoad is not the same as opposite's "
			            "A.crossRoad";
			valid = false;
		}
		if (wall.first.b.crossRoad != wall.second.b.crossRoad) {
			JUST_LOG << "pointB's crossRoad is not the same as opposite's "
			            "B.crossRoad";
			valid = false;
		}

		try {
			// first or second - doesn't matter, they're equal
			validate_cross_road(wall.first.a.crossRoad);
			validate_cross_road(wall.first.b.crossRoad);
		} catch (const char* err) {
			LOG_S(ERROR) << err;
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
		float dist = calcSquaredLen(point.point, point_to_insert.point),
		      max_allowed = pythagoras(point.width, point_to_insert.width);

		if (dist <= max_allowed) {
			cr.points.push_back(point_to_insert);
			return true;
		}
	}
	return false;
}

void add_a_single_way_to_maze(PWalls& wallsP, const WideRoad2& way,
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

	wallsP.push_back({_upper, _lower});
	Segment2& upper = wallsP.back().first;

	Segment2& lower = wallsP.back().second;

	cross_roads.push_back({{way.a}});
	lower.a.crossRoad = upper.a.crossRoad = &cross_roads.back();

	cross_roads.push_back({{way.b}});
	lower.b.crossRoad = upper.b.crossRoad = &cross_roads.back();

	LOG_F(INFO, "NEXT INSERT START: %s/%s", upper.color.name, lower.color.name);

	for (auto& wall : wallsP) {
		if (&wall.first == &upper) continue;

		JUST_LOG << "\tCHECK WITH " << wall.first.color << "/"
		         << wall.second.color << ":";
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

		// LOG_S(INFO) << iupper << ilower << iupperOpposite << ilowerOpposite;

		// middle-middle intersection
		if (iupper && ilower && iupperOpposite && ilowerOpposite) {
			LOG_F(INFO, "\tmiddle-middle");
			{
				CrossRoad _croad{};
#pragma message("Calculate WidePoint2 width properly")
				_croad.points.push_back(
				    {middleOf({ipLower, ipUpper}), WIDEPOINT_WIDTH});
				_croad.points.push_back(
				    {middleOf({ipLowerOpposite, ipUpperOpposite}),
				     WIDEPOINT_WIDTH});
				_croad.points.push_back(
				    {middleOf({ipLower, ipLowerOpposite}), WIDEPOINT_WIDTH});
				_croad.points.push_back(
				    {middleOf({ipUpper, ipUpperOpposite}), WIDEPOINT_WIDTH});

				cross_roads.push_back(_croad);
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
			wallsP.push_back({new_upper, new_lower});

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

			wallsP.push_back({new_upper_other, new_lower_other});

			// CUT THIS AND OTHER
			upper.b.moveTo(
			    Segment2{ipUpper, ipUpperOpposite}.getEndCloserTo(upper.a));
			lower.b.moveTo(
			    Segment2{ipLower, ipLowerOpposite}.getEndCloserTo(lower.a));

			upper.b.crossRoad = &croad;
			lower.b.crossRoad = &croad;

			wall.first.b.moveTo(
			    Segment2{ipLower, ipUpper}.getEndCloserTo(wall.first.a));
			wall.second.b.moveTo(
			    Segment2{ipLowerOpposite, ipUpperOpposite}.getEndCloserTo(
			        wall.second.a));

			wall.first.b.crossRoad = &croad;
			wall.second.b.crossRoad = &croad;
		} else { // try to insert that point of lower/upper which is closer to
			     // the crossPoint to the crossRoad of that point of
			     // wall/opposite which is closer to the crossPoint. If yes, we
			     // have an end-to-end connection
			bool this_join_a =
			         (&way.getEndCloserTo(
			              ilower ? ipLower : iupper ? ipUpper
			                                        : ilowerOpposite
			                                              ? ipLowerOpposite
			                                              : ipUpperOpposite) ==
			          &way.a),
			     other_join_a =
			         (&wall.first.getEndCloserTo(ipLower) == &wall.first.a);

			CrossRoad* other_closer_croad =
			    wall.first.getEndCloserTo(ilower ? ipLower : ipUpper).crossRoad;

			if (tryToInsert(*other_closer_croad, this_join_a ? way.a : way.b)) {
				LOG_F(3, "\tend-end");

				if ((this_join_a ? upper.a : upper.b).crossRoad->points.size() <
				    2) {
					cross_roads.remove(
					    *(this_join_a ? upper.a : upper.b).crossRoad);
				}

				(this_join_a ? upper.a : upper.b).crossRoad =
				    (this_join_a ? lower.a : lower.b).crossRoad =
				        other_closer_croad;

				if (!iupperOpposite && !ilowerOpposite) {
					if (iupper && !ilower) {
						// cut at iupper
						upper.getEndCloserTo(ipUpper).moveTo(ipUpper);
						wall.first.getEndCloserTo(ipUpper).moveTo(ipUpper);
					} else if (ilower && !iupper) {
						// cut at ilower
						lower.getEndCloserTo(ipLower).moveTo(ipLower);
						wall.first.getEndCloserTo(ipLower).moveTo(ipLower);
					} else {
						LOG_F(WARNING, "YEP TO DO!");
					}
				} else if (!iupper && !ilower) {
					if (iupperOpposite && !ilowerOpposite) {
						// cut at iupperOpposite
						upper.getEndCloserTo(ipUpperOpposite)
						    .moveTo(ipUpperOpposite);
						wall.second.getEndCloserTo(ipUpperOpposite)
						    .moveTo(ipUpperOpposite);
					} else if (ilowerOpposite && !iupperOpposite) {
						// cut at ilowerOpposite
						lower.getEndCloserTo(ipLowerOpposite)
						    .moveTo(ipLowerOpposite);
						wall.second.getEndCloserTo(ipLowerOpposite)
						    .moveTo(ipLowerOpposite);
					} else {
						// opposite crossed by both upper & lower,
						// not opposite not crossed by any of them
						const Point2& cross_point =
						    Segment2{ipLowerOpposite, ipUpperOpposite}
						        .getEndCloserTo(other_join_a ? wall.second.b
						                                     : wall.second.a);

						(other_join_a ? wall.second.a : wall.second.b)
						    .moveTo(cross_point);
						Segment2& cut_wall_this =
						    cross_point == ipUpperOpposite ? upper : lower;
						(this_join_a ? cut_wall_this.a : cut_wall_this.b)
						    .moveTo(cross_point);
					}
				} else {
					LOG_S(ERROR) << "Don't know what to do";
					throw "No idea.";
				}
			} else { // otherwise, we have a middle-to-end join
				const bool iu = iupper, il = ilower, iuo = iupperOpposite,
				           ilo = ilowerOpposite;

				LOG_F(INFO, "\tMiddle-end");

				bool this_end_joins_2_fixed_points_intersect =
				         (iu && il && !iuo && !ilo) ||
				         (iuo && ilo && !iu && !il),
				     this_end_joins_2_fixed_points_on_the_one_side =
				         (iu && iuo && !il && !ilo) ||
				         (il && ilo && !iu && !iuo),
				     only_one_point_is_not_part_of_intersection =
				         (!iu && iuo && il && ilo) ||
				         (iu && !iuo && il && ilo) ||
				         (iu && iuo && !il && ilo) || (iu && iuo && il && !ilo),
				     only_one_point_intersects = (1 == iu + il + iuo + ilo);

				bool this_joins_other =
				    this_end_joins_2_fixed_points_intersect ||
				    this_end_joins_2_fixed_points_on_the_one_side;

				auto point_inside_segment = [](Segment2 line, Point2 point) {
					double x = point.x, x1 = line.a.x, x2 = line.b.x,
					       y = point.y, y1 = line.a.y, y2 = line.b.y;
					// http://stackoverflow.com/a/17590923
					double AB =
					    sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					double AP = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
					double PB = sqrt((x2 - x) * (x2 - x) + (y2 - y) * (y2 - y));
					if (AB == AP + PB) return true;
					return false;
				};

				if (only_one_point_is_not_part_of_intersection) {
					bool qq = only_one_point_is_not_part_of_intersection;
					bool try_lower = false;
					Point2* point_to_check = nullptr;

					if (qq == iu) {
						point_to_check = &ipUpper;
					}
					if (qq == il) {
						try_lower = true;
						point_to_check = &ipLower;
					}
					if (qq == iuo) {
						// try_opposite = true;
						point_to_check = &ipUpperOpposite;
					}
					if (qq == ilo) {
						try_lower = true;
						// try_opposite = true;
						point_to_check = &ipLowerOpposite;
					}

					if (!point_inside_segment(try_lower ? lower : upper,
					                          *point_to_check)) {
						// this joins other
						this_joins_other = true;
					}
				}

				if (only_one_point_intersects) {
					if (iu) {
						if (!point_inside_segment(lower, ipLower)) {
							this_joins_other = true;
						}
					} else if (il) {
						if (!point_inside_segment(upper, ipUpper)) {
							this_joins_other = true;
						}
					} else if (iuo) {
						if (!point_inside_segment(lower, ipLowerOpposite)) {
							this_joins_other = true;
						}
					} else {
						LOG_F(WARNING, "unimplemented!");
						// too lazy now
						throw "Unimplemented.";
					}
				}

				// other end -> this middle
				if (!this_joins_other) {
					VLOG_F(2, "\tOther's end joins this's middle");
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

					wallsP.push_back({new_upper, new_lower});

					wallsP.back().first.a.crossRoad =
					    other_cut_a ? wall.first.a.crossRoad
					                : wall.first.b.crossRoad;
					wallsP.back().second.a.crossRoad =
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

				} else {
					// end of this joining middle of other
					LOG_F(2, "This's end joins other's middle");
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

					wallsP.push_back({new_first, new_second});
					wallsP.back().first.a.crossRoad =
					    this_cut_a ? upper.a.crossRoad : upper.b.crossRoad;
					wallsP.back().second.a.crossRoad =
					    this_cut_a ? lower.a.crossRoad : lower.b.crossRoad;

					wall.first.b.moveTo(Segment2{
					    ipUpper, ipLower}.getEndCloserTo(wall.first.a));
					wall.second.b.moveTo(Segment2{
					    ipUpperOpposite,
					    ipLowerOpposite}.getEndCloserTo(wall.second.a));

					(this_cut_a ? upper.a : upper.b)
					    .moveTo(joins_from_opposite ? ipUpperOpposite
					                                : ipUpper);

					(this_cut_a ? lower.a : lower.b)
					    .moveTo(joins_from_opposite ? ipLowerOpposite
					                                : ipLower);
				}
			}
		}
	}

	FORCE_VALIDATE&& LOG_F(INFO, "Will validate.");
	validate_walls(wallsP);
}

void build_from_paths(const Ways& paths, std::list<Segment2>& maze)
{
	loguru::g_stderr_verbosity = 3;
	loguru::g_colorlogtostderr = true;


	CrossRoads cross_roads;

	PWalls wallsP;

	for (const auto& way : paths) {
		if (way.a.width != WIDEPOINT_WIDTH || way.b.width != WIDEPOINT_WIDTH) {
			JUST_LOG << way.a.point << " - " << way.b.point
			         << "; a.width: " << way.a.width
			         << ", b.width: " << way.b.width;
			throw "WidePoint's width must be 3(or another fixed value "
			      "specified by WIDEPOINT_WIDTH) until other problems are "
			      "resolved";
		}
		add_a_single_way_to_maze(wallsP, way, cross_roads);
	}

    //auto crsize = cross_roads.size();
    //std::vector<Segment2> cross_points_segments;
    //cross_points_segments.reserve(crsize*(crsize-1)/2);
    //for (auto c : cross_roads) {
        //for(auto i=c.points.cbegin(); i != c.points.cend(); ++i) {
            //for(auto p=i; p!= c.points.cend(); ++p) {
               //maze.push_back({i->point, p->point, colors[0]});
            //}
        //}
    //}

	for (auto i : wallsP) {
		maze.push_back(i.first);
		maze.push_back(i.second);
	}

	LOG_F(INFO, "Ways added to maze. Will validate.");

	validate_walls(wallsP, true);

	LOG_F(INFO, "%lu walls generated from %lu lines\n", maze.size(),
	      paths.size());

	dump(wallsP);
	LOG_F(INFO, "CrossRoads in std::list cross_roads: %lu\n",
	      cross_roads.size());
}
