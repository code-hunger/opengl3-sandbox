#ifndef GEOMETRY_H_NTEYZYI7
#define GEOMETRY_H_NTEYZYI7

#include "types.h"
#include <algorithm>

namespace math {

using std::sqrt;

inline constexpr float pythagoras(float a, float b) { return a * a + b * b; }

inline constexpr float calcSquaredLen(const Point2& a, const Point2& b)
{
	return pythagoras(a.x - b.x, a.y - b.y);
}

inline constexpr float calcSquaredLen(const WidePoint2& a, const Point2& b)
{
	return pythagoras(a.point.x - b.x, a.point.y - b.y);
}

inline constexpr float calcSquaredLen(const Segment2& segment)
{
	return calcSquaredLen(segment.a, segment.b);
}

inline constexpr float calcSquaredLen(const WideRoad2& wideRoad)
{
	return calcSquaredLen(wideRoad.a.point, wideRoad.b.point);
}

inline constexpr LineEquation getEquation(const Segment2& segment)
{
	const Point2 &a = segment.a, &b = segment.b;
	float x_coeff = (a.y - b.y) / (a.x - b.x);
	return {x_coeff, a.y - x_coeff * a.x};
}

inline constexpr auto intersect(const Segment2& one, const Segment2& other)
{
	const LineEquation eThis = getEquation(one), eOther = getEquation(other);

	const float crossX = (eOther.b - eThis.b) / (eThis.a - eOther.a),
	            crossY = eThis.a * crossX + eThis.b;

	struct
	{
		Point2 point;
		bool intersects = true;
	} result{{crossX, crossY}};

	if ((crossX < one.b.x && crossX > one.a.x) ||
	    (crossX > one.b.x && crossX < one.a.x)) {
		// this crossX yes
	} else {
		result.intersects = false;
	}
	if ((crossX < other.b.x && crossX > other.a.x) ||
	    (crossX > other.b.x && crossX < other.a.x)) {
		// other crossX yes
	} else {
		result.intersects = false;
	}
	if ((crossY < one.b.y && crossY > one.a.y) ||
	    (crossY > one.b.y && crossY < one.a.y)) {
		// this crossY yes
	} else {
		result.intersects = false;
	}
	if ((crossY < other.b.y && crossY > other.a.y) ||
	    (crossY > other.b.y && crossY < other.a.y)) {
		// other crossY yes
	} else {
		result.intersects = false;
	}

	return result;
}

template <typename T> constexpr T abs(T x) { return x > 0 ? x : -x; }

constexpr inline bool insideSegment(const Point2& point, const Segment2& line)
{
	// http://stackoverflow.com/a/17590923
	double AB = calcSquaredLen(line.a, line.b),
	       AP = calcSquaredLen(line.a, point),
	       PB = calcSquaredLen(line.b, point);
	double res = sqrt(AP) + sqrt(PB) - sqrt(AB);
	return abs(res) < 1e-5;
}

template <typename T> constexpr T fabs(T a) { return a < 0 ? -a : a; }
template <typename T> constexpr T pow2(T a) { return a * a; }

// The distance from a point to a line defined by 2 points (i.e. a segment)
inline constexpr double distanceToLine(const Point2& point,
                                       const Segment2& line)
{
	// http://paulbourke.net/geometry/pointlineplane/
	float x3 = point.x, x1 = line.a.x, x2 = line.b.x;
	float y3 = point.y, y1 = line.a.y, y2 = line.b.y;

	float u =
	    ((x3 - x1) * (x2 - x1) + (y3 - y1) * (y2 - y1)) / calcSquaredLen(line);

	const Point2& other_proj =
	    (u < 0
	         ? line.a
	         : u > 1 ? line.b : Point2{x1 + u * (x2 - x1), y1 + u * (y2 - y1)});

	return sqrt(calcSquaredLen(point, other_proj));
}

inline constexpr double distanceToLine(const Segment2& a, const Segment2& b)
{
	return std::min({distanceToLine(b.a, a), distanceToLine(b.b, a),
	                 distanceToLine(a.a, b), distanceToLine(a.b, b)});
}

inline constexpr float get_width_at_point(const WideRoad2& way,
                                          const Point2& ip)
{
	const WidePoint2 &a = (way.a.width < way.b.width ? way.a : way.b),
	                 &b = (&a == &way.a ? way.b : way.a);
	return sqrt(calcSquaredLen(a, ip)) * (b.width - a.width) /
	           sqrt(calcSquaredLen(way)) +
	       a.width;
}

inline constexpr Point2 middleOf(Point2 a, Point2 b)
{
	return {a.x + (b.x - a.x) / 2, a.y + (b.y - a.y) / 2};
}

inline constexpr Point2 middleOf(Segment2 line)
{
	return middleOf(line.a, line.b);
}

template <typename S, typename P>
inline constexpr auto& getEndCloserTo(S& segment, P& point)
{
	auto &a = segment.a, &b = segment.b;
	return calcSquaredLen(a, point) < calcSquaredLen(b, point) ? a : b;
}

template <>
inline constexpr auto& getEndCloserTo<WideRoad2&, Segment2&>(WideRoad2& road,
                                                             Segment2& segment)
{
	double distA = distanceToLine(road.a.point, segment),
	       distB = distanceToLine(road.b.point, segment);
	return distA < distB ? road.a : road.b;
}

inline ColorSegment2 toColorSegment(const Segment2& way,
                                    Color color = Color::next())
{
	return {{way.a, way.b}, color};
}

inline ColorSegment2 toColorSegment(const WideRoad2& way,
                                    Color color = Color::next())
{
	return {{way.a.point, way.b.point}, color};
}

inline ColorSegment2 toWhiteSegment2(const Segment2& way)
{
	return toColorSegment(way, {0.7f, 0.7f, 0.7f, "white"});
}

inline ColorSegment2 toWhiteSegment2(const WideRoad2& way)
{
	return toColorSegment(way, {0.7f, 0.7f, 0.7f, "white"});
}

const Color& Color::next()
{
	static const std::vector<Color> colors = {
	    {1, 1, 0.1f, "L yellow"},    {0, 0.3f, 1.f, "D blue"},
	    {0.5f, 0, 0.8f, "violet"},   {1, 0.5f, 0, "orange"},
	    {.5f, .5f, .5f, "silver"},   {1.f, 0, 0, "red"},
	    {1.f, 0.5f, 0.5f, "pink"},   {0, .7f, 1.f, "L blue"},
	    {0.7f, 0.2f, 0.3f, "brown"}, {0.2f, 1, 0.2f, "L green"},
	    {0, 0.5f, 0, "D green"}};
	static unsigned long color_count = colors.size(), color = 0;

	return colors[(color++) % color_count];
}

// struct Hash
//{
// size_t operator()(const WideRoad2& way) const
//{
// return static_cast<size_t>(
// static_cast<float>(way.a.width * way.b.width) +
// way.getSegmnet2().calcSquaredLen());
//}

// size_t operator()(const Segment2& line) const
//{
// return static_cast<size_t>(10000 * line.a.x * line.a.y +
// line.calcSquaredLen());
//}
//};
}

#endif /* end of include guard: GEOMETRY_H_NTEYZYI7 */
