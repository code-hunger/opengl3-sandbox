#ifndef GEOMETRY_H_NTEYZYI7
#define GEOMETRY_H_NTEYZYI7

#include "types.h"
#include <algorithm>

namespace math {

using std::sqrt;

inline constexpr float pythagoras(float a, float b) { return a * a + b * b; }

inline constexpr float Point2::distance2(const Point2& p) const
{
	return pythagoras(x - p.x, y - p.y);
}

inline constexpr float Segment2::len2() const { return a.distance2(b); }

inline float WideRoad2::len2() const
{
	return a.point.distance2(b.point);
}

inline constexpr LineEquation Segment2::equation() const
{
	float x_coeff = (a.y - b.y) / (a.x - b.x);
	return {x_coeff, a.y - x_coeff * a.x};
}

inline constexpr auto intersect(const Segment2& one, const Segment2& other)
{
	const LineEquation eThis = one.equation(), eOther = other.equation();

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

constexpr inline bool Segment2::isInside(const Point2& point) const
{
	// http://stackoverflow.com/a/17590923
	// check if <a,b,point> is a triangle by checking the length of its sides
	float AP = a.distance2(point), BP = b.distance2(point);
	return abs(sqrt(AP) + sqrt(BP) - sqrt(len2())) < 1e-5f;
}

template <typename T> constexpr T fabs(T a) { return a < 0 ? -a : a; }
template <typename T> constexpr T pow2(T a) { return a * a; }

// The distance from a point to a line defined by 2 points (i.e. a segment)
inline constexpr float Point2::distance2(const Segment2& line) const
{
	// http://paulbourke.net/geometry/pointlineplane/
	float x3 = x, x1 = line.a.x, x2 = line.b.x;
	float y3 = y, y1 = line.a.y, y2 = line.b.y;

	float u = ((x3 - x1) * (x2 - x1) + (y3 - y1) * (y2 - y1)) / line.len2();

	const Point2& other_proj =
	    (u < 0
	         ? line.a
	         : u > 1 ? line.b : Point2{x1 + u * (x2 - x1), y1 + u * (y2 - y1)});

	return distance2(other_proj);
}

inline double distanceToLine(const Segment2& a, const Segment2& b)
{
	return sqrt(std::min({b.a.distance2(a), b.b.distance2(a),
	                      a.a.distance2(b), a.b.distance2(b)}));
}

inline float WideRoad2::widthAt(const Point2& ip) const
{
	const WidePoint2 &narrow = (a.width < b.width ? a : b),
	                 &wider = (&narrow == &a ? b : a);
	return sqrt(narrow.point.distance2(ip)) *
	           (wider.width - narrow.width) / sqrt(len2()) +
	       narrow.width;
}

inline constexpr Point2 middleOf(Point2 a, Point2 b)
{
	return {a.x + (b.x - a.x) / 2, a.y + (b.y - a.y) / 2};
}

inline constexpr Point2 middleOf(Segment2 line)
{
	return middleOf(line.a, line.b);
}

inline const Point2& Segment2::getEndCloserTo(const Point2& point) const
{
	return point.distance2(a) < point.distance2(b)? a : b;
}

inline WidePoint2& WideRoad2::getEndCloserTo(const Point2& point)
{
	return point.distance2(a.point) < point.distance2(b.point)
	           ? a
	           : b;
}

inline constexpr auto& getEndCloserTo(WideRoad2& road, Segment2& segment)
{
	auto distA = road.a.point.distance2(segment),
	     distB = road.b.point.distance2(segment);
	return distA < distB ? road.a : road.b;
}

inline ColorSegment2 Segment2::colorSegment(Color color ) const
{
	return {{a, b}, color};
}

inline ColorSegment2 WideRoad2::colorSegment(Color color) const
{
	return {{a.point, b.point}, color};
}

inline ColorSegment2 Segment2::whiteSegment() const
{
	return colorSegment({0.7f, 0.7f, 0.7f, "white"});
}

inline ColorSegment2 WideRoad2::whiteSegment() const
{
	return colorSegment({0.7f, 0.7f, 0.7f, "white"});
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
// way.getSegmnet2().len2());
//}

// size_t operator()(const Segment2& line) const
//{
// return static_cast<size_t>(10000 * line.a.x * line.a.y +
// line.len2());
//}
//};
}

#endif /* end of include guard: GEOMETRY_H_NTEYZYI7 */
