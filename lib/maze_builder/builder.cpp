#include "graphics/geometry_io.h"
#define BOOST_LOG_DYN_LINK

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "easylogging++.h"
#pragma GCC diagnostic pop

INITIALIZE_EASYLOGGINGPP
#define ERR LOG(ERROR)
#define NFO LOG(INFO)

#include <cassert>
#include <cmath>
#define PI 3.141592653589793238462643383279502884L

#include <list>
#include <set>
#include <unordered_set>
#include <vector>

#include <iostream>
using std::cout;
using std::endl;

typedef std::unordered_set<WideRoad2, Hash> Ways;
typedef std::list<std::pair<Segment2, Segment2>> Walls;
typedef std::vector<Color> Colors;
typedef std::list<CrossRoad> CrossRoads;

#define FORCE_VALIDATE false

constexpr int WIDEPOINT_WIDTH = 3;

el::Logger& log()
{
	static el::Logger* logger = el::Loggers::getLogger("default");
	return *logger;
}

inline unsigned long get_ptr_val(void* ptr)
{
	return reinterpret_cast<unsigned long>(ptr) % 10000l;
}

// It's wrong. To be removed
// @Deprecated
inline void dump(Walls walls)
{
	std::set<CrossRoad*> count_unique;
	for (const auto& _p : walls) {

		{
			auto& p = _p.first;
			count_unique.insert(p.a.crossRoad);
			count_unique.insert(p.b.crossRoad);

			NFO << p.color << " " << p.a << "(" << get_ptr_val(p.a.crossRoad)
			    << ") : " << p.a.crossRoad->points[0];
			NFO << p.b << "(" << get_ptr_val(p.b.crossRoad)
			    << ") : " << p.b.crossRoad->points[0];
		}
		{
			auto& p = _p.second;
			count_unique.insert(p.a.crossRoad);
			count_unique.insert(p.b.crossRoad);

			NFO << p.color << " " << p.a << "(" << get_ptr_val(p.a.crossRoad)
			    << ") : " << p.a.crossRoad->points[0];
			NFO << p.b << "(" << get_ptr_val(p.b.crossRoad)
			    << ") : " << p.b.crossRoad->points[0];
		}
	}
	NFO << "Unique crossRoads: " << count_unique.size();
}

void validate_cross_road(CrossRoad* crossRoad)
{
	if (crossRoad == nullptr) {
		throw "CrossRoad pointer is empty!";
	}
	if (crossRoad->points.size() < 1) {
		throw "CrossRoads should have at least one point!";
	}
	//if (crossRoad->lines.size() < 2) {
		//throw "CrossRoads can't have less than 2 lines!";
	//}
}

void validate_walls(Walls& walls, bool force = FORCE_VALIDATE)
{
	if (!force) return;

	NFO << "Validating walls...";
	bool valid = true;
	for (const auto& wall : walls) {
		// First check the opposite
		/*if (wall.opposite == nullptr) {
			ERR << "\nOpposite is nullptr: " << wall.segment;
			valid = false;
		} else if (wall.opposite->opposite != &wall) {
			ERR << "\nThe opposite's opposite is not the current wall!";
			valid = false;
		} else {*/
			if (wall.first.a.crossRoad !=
			    wall.second.a.crossRoad) {
				ERR << "pointA's crossRoad is not the same as opposite's "
				       "A.crossRoad";
				valid = false;
			}
			if (wall.first.b.crossRoad !=
			    wall.second.b.crossRoad) {
				ERR << "pointB's crossRoad is not the same as opposite's "
				       "B.crossRoad";
				valid = false;
			}
		/*}*/

		try {
			// first or not - doesn't matter, they're equal
			validate_cross_road(wall.first.a.crossRoad);
			validate_cross_road(wall.first.b.crossRoad);
		} catch (const char* err) {
			ERR << endl << err << endl;
			valid = false;
		}
	}
	if (!valid) {
		throw "Walls' state invalid!";
	}
	NFO << " ready!";
}

bool tryToInsert(CrossRoad& cr, WidePoint2 point_to_insert)
{
	if (cr.points.size() < 1)
		throw "CrossRoad MUST have at least 1 point inside!";

	for (const auto& point : cr.points) {
		float dist = calcSquaredLen(point.point, point_to_insert.point),
		      max_allowed = pythagoras(point.width, point_to_insert.width);
		// NFO << point << ' ' << point_to_insert << " Dist = " << dist << ",
		// max allowed: " << max_allowed << '\n';
		if (dist <= max_allowed) {
			cr.points.push_back(point_to_insert);
			// NFO << "Push back two points that are near!";
			return true;
		}
	}
	return false;
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

	wallsP.push_back({_upper, _lower});
	Segment2& upper = wallsP.back().first;

	Segment2& lower = wallsP.back().second;

	cross_roads.push_back({{&lower, &upper}, {way.a}});
	lower.a.crossRoad = upper.a.crossRoad = &cross_roads.back();

	cross_roads.push_back({{&lower, &upper}, {way.b}});
	lower.b.crossRoad = upper.b.crossRoad = &cross_roads.back();

	log().info("NEXT INSERT START: %v/%v", upper.color.name,
	           lower.color.name);

	// unsigned counter=0;
	for (auto& wall : wallsP) {
		if (&wall.first == &upper) continue;
		// if(counter++ % 2) continue;

		//if (wall.disable_intersect_check) {
			//wall.disable_intersect_check = 0;
			//continue;
		//};
		Segment2 &segment = wall.first, &opposite = wall.second;
		NFO << "CHECK WITH " << segment.color << "/" << opposite.color << ":";
		Point2 ipUpper{}, ipLower{}, ipUpperOpposite{}, ipLowerOpposite{};
		const bool iupper = segment.intersectsWith(upper, &ipUpper),
		           ilower = segment.intersectsWith(lower, &ipLower),
		           ilowerOpposite =
		               opposite.intersectsWith(lower, &ipLowerOpposite),
		           iupperOpposite =
		               opposite.intersectsWith(upper, &ipUpperOpposite);

		// nothing intersecting here...
		if (!(iupper || ilower || iupperOpposite || ilowerOpposite)) {
		}

		// middle-middle intersection
		if (iupper && ilower && iupperOpposite && ilowerOpposite) {
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
			    Segment2{ipLower, ipLowerOpposite}.getEndCloserTo(
			        lower.b),
			    lower.b};
			Segment2 new_upper = {
			    Segment2{ipUpper, ipUpperOpposite}.getEndCloserTo(
			        upper.b),
			    upper.b};

			new_upper.a.crossRoad = &croad;
			new_upper.b.crossRoad = upper.b.crossRoad;

			new_lower.a.crossRoad = &croad;
			new_lower.b.crossRoad = lower.b.crossRoad;

			new_lower.color = colors[color];
			new_upper.color = colors[(color + 1) % color_count];
			color = (color + 2) % color_count;

			wallsP.push_back({new_upper, new_lower});

			// OTHER COMPLEMENTING WALLS
			Segment2 new_lower_other = {
			    Segment2{ipUpper, ipLower}.getEndCloserTo(wall.first.b),
			    wall.first.b};
			Segment2 new_upper_other = {
			    Segment2{ipUpperOpposite, ipLowerOpposite}.getEndCloserTo(
			        wall.second.b),
			    wall.second.b};

			new_upper_other.a.crossRoad = &croad;
			new_upper_other.b.crossRoad = wall.first.b.crossRoad;

			new_lower_other.a.crossRoad = &croad;
			new_lower_other.b.crossRoad = wall.second.b.crossRoad;

			new_lower_other.color = colors[color];
			new_upper_other.color = colors[(color + 1) % color_count];
			color = (color + 2) % color_count;

			wallsP.push_back({new_upper_other, new_lower_other});

			// CUT THIS AND OTHER
			upper.b.moveTo(
			    Segment2{ipUpper, ipUpperOpposite}.getEndCloserTo(
			        upper.a));
			lower.b.moveTo(
			    Segment2{ipLower, ipLowerOpposite}.getEndCloserTo(
			        lower.a));

			upper.b.crossRoad = &croad;
			lower.b.crossRoad = &croad;

			wall.first.b.moveTo(
			    Segment2{ipLower, ipUpper}.getEndCloserTo(wall.first.a));
			wall.second.b.moveTo(
			    Segment2{ipLowerOpposite, ipUpperOpposite}.getEndCloserTo(
			        wall.second.a));

			wall.first.b.crossRoad = &croad;
			wall.second.b.crossRoad = &croad;
			// break;
		} else { // try to insert that point of lower/upper which is closer to
			     // the crossPoint to the crossRoad of that point of
			     // wall/opposite which is closer to the crossPoint. If yes, we
			     // have an end-to-end connection
			bool this_join_a = (&way.getEndCloserTo(ipLower) == &way.a);
			if ((iupper || ilower || iupperOpposite || ilowerOpposite) &&
			    tryToInsert(
			        *wall.first.getEndCloserTo(ilower ? ipLower : ipUpper)
			             .crossRoad,
			        this_join_a ? way.a : way.b)) {

				if (!iupperOpposite && !ilowerOpposite) {
					if (iupper) {
						// cut at iupper
						LOG(WARNING) << "iU: " << upper.color << " " << wall.first.color;;
						upper.getEndCloserTo(ipUpper).moveTo(ipUpper);
						wall.first.getEndCloserTo(ipUpper).moveTo(ipUpper);
					} else {
						// cut at ilower
						LOG(WARNING) << "iL: "<< lower.color << " " << wall.first.color;;
						lower.getEndCloserTo(ipLower).moveTo(ipLower);
						wall.first.getEndCloserTo(ipLower).moveTo(ipLower);
					}
				} else if (!iupper && !ilower) {
					if (iupperOpposite) {
						// cut at iupperOpposite
						LOG(WARNING) << "iUO: " << upper.color << " " << wall.second.color;
						upper.getEndCloserTo(ipUpperOpposite)
						    .moveTo(ipUpperOpposite);
						wall.second.getEndCloserTo(ipUpperOpposite)
						    .moveTo(ipUpperOpposite);
						NFO << ipUpper << " : " << ipUpperOpposite << endl;
					} else {
						 //cut at ilowerOpposite
						LOG(WARNING) << "iLO: "<< lower.color << " " << wall.second.color;;
						lower.getEndCloserTo(ipLowerOpposite)
							.moveTo(ipLowerOpposite);
						wall.second.getEndCloserTo(ipLowerOpposite)
							.moveTo(ipLowerOpposite);
						NFO << ipLower << " : " << ipLowerOpposite << endl;
					}
				} else {
					throw "This couldn't be!!! More than one point does not "
					      "intersect! (It's supposed that exactly one point is "
					      "not intersecting)";
				}
			} else { // otherwise, we have a middle-to-end join
				bool i_this = iupper || ilower,
					 i_other = iupperOpposite || ilowerOpposite;

				NFO << "i this: " << i_this << ", i other: " << i_other;

				if(!i_this && i_other) {
					// end of other joining middle of this
					LOG(WARNING) << "Other's end joins this's middle";
					//wall.first.getEndCloserTo(ipLower).moveTo(ipLower);
					//wall.second.getEndCloserTo(ipLowerOpposite).moveTo(ipLowerOpposite);
				} else if (i_this && !i_other){
					// end of this joining middle of other
					LOG(WARNING) << "This's end joins other's middle";
				} else if((iupper && iupperOpposite) or (ilower && ilowerOpposite)) {
					LOG(WARNING) << "Other's end joins this's middle";
				} else {
					ERR << ":///";
				}
			}
		}
	}

	validate_walls(wallsP);
}

void build_from_paths(const Ways& paths, Walls& maze)
{
	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

	CrossRoads cross_roads;
	Colors colors = {{1, 1, 0.1f, "L yellow"},    {0, 0.3f, 1.f, "D blue"},
	                 {0.5f, 0, 0.8f, "violet"},   {1, 0.5f, 0, "orange"},
	                 {.5f, .5f, .5f, "silver"},   {1.f, 0, 0, "red"},
	                 {1.f, 0.5f, 0.5f, "pink"},   {0, .7f, 1.f, "L blue"},
	                 {0.7f, 0.2f, 0.3f, "brown"}, {0.2f, 1, 0.2f, "L green"},
	                 {0, 0.5f, 0, "D green"}};

	auto color_count = colors.size(), color = 0lu;

	for (const auto& way : paths) {
		if (way.a.width != WIDEPOINT_WIDTH || way.b.width != WIDEPOINT_WIDTH) {
			ERR << way.a.point << " - " << way.b.point
			    << "; a.width: " << way.a.width << ", b.width: " << way.b.width;
			throw "WidePoint's width must be 3(or another fixed value "
			      "specified by WIDEPOINT_WIDTH) until other problems are "
			      "resolved";
		}
		add_a_single_way_to_maze(maze, way, colors, color_count, color,
		                         cross_roads);
	}

	validate_walls(maze, true);

	log().info("%v walls generated from %v lines\n", maze.size(), paths.size());

	dump(maze);
	log().info("CrossRoads in std::list cross_roads: %v\n", cross_roads.size());
}
