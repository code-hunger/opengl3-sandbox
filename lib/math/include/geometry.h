#ifndef GEOMETRY_H_NTEYZYI7
#define GEOMETRY_H_NTEYZYI7

#include <cstddef>
#include <vector>

struct CrossRoad;

struct Color
{
	float r, g, b;
	char const* name;
	operator const char*() const { return name; }

	static Color next()
	{
		static std::vector<Color> colors = {
		    {1, 1, 0.1f, "L yellow"},    {0, 0.3f, 1.f, "D blue"},
		    {0.5f, 0, 0.8f, "violet"},   {1, 0.5f, 0, "orange"},
		    {.5f, .5f, .5f, "silver"},   {1.f, 0, 0, "red"},
		    {1.f, 0.5f, 0.5f, "pink"},   {0, .7f, 1.f, "L blue"},
		    {0.7f, 0.2f, 0.3f, "brown"}, {0.2f, 1, 0.2f, "L green"},
		    {0, 0.5f, 0, "D green"}};
		static auto color_count = colors.size(), color = 0lu;

		return colors[(color++) % color_count];
	}
};

struct Segment2;

struct Point2
{
	float x, y;
	CrossRoad* crossRoad = nullptr;

	inline bool operator==(const Point2& other) const
	{
		return this->x == other.x && this->y == other.y;
	}

	operator bool() = delete;

	// Don't allow this - *crossRoad must be preserved
	void operator=(const Point2&) = delete;

	// This only moves coordinates, doesn't copy *crossRoad
	Point2& moveTo(const Point2& other)
	{
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	bool insideSegment(Segment2 const& line) const;
};

struct Vector2
{
	int x, y;
};

struct Point3
{
	int x, y, z;
};

struct Vector3
{
	int x, y, z;
};

struct LineEquation
{
	float a, b;
};

inline constexpr float pythagoras(float a, float b) { return a * a + b * b; }

inline constexpr float calcSquaredLen(const Point2& a, const Point2& b)
{
	return pythagoras(a.x - b.x, a.y - b.y);
}

double distanceToLine(Point2, Segment2);

struct Segment2
{
	Point2 a, b;
	Color color{};

	bool operator==(const Segment2& other) const
	{
		return (a == other.a && b == other.b) || (a == other.b && b == other.a);
	}

	std::pair<bool, Point2> intersectsWith(Segment2 const& other) const
	{
		LineEquation eThis = getEquation(), eOther = other.getEquation();

		float crossX = (eOther.b - eThis.b) / (eThis.a - eOther.a),
		      crossY = eThis.a * crossX + eThis.b;

		bool intersects = true;

		if ((crossX < b.x && crossX > a.x) || (crossX > b.x && crossX < a.x)) {
			// this crossX yes
		} else {
			intersects = false;
		}
		if ((crossX < other.b.x && crossX > other.a.x) ||
		    (crossX > other.b.x && crossX < other.a.x)) {
			// other crossX yes
		} else {
			intersects = false;
		}
		if ((crossY < b.y && crossY > a.y) || (crossY > b.y && crossY < a.y)) {
			// this crossY yes
		} else {
			intersects = false;
		}
		if ((crossY < other.b.y && crossY > other.a.y) ||
		    (crossY > other.b.y && crossY < other.a.y)) {
			// other crossY yes
		} else {
			intersects = false;
		}
		return {intersects, {crossX, crossY}};
	}

	float calcSquaredLen() const { return ::calcSquaredLen(a, b); }

	LineEquation getEquation() const
	{
		float a = (this->a.y - b.y) / (this->a.x - b.x);
		return {a, this->a.y - a * this->a.x};
	}

	// NOT const - we may want to change the refference later
	Point2& getEndCloserTo(const Point2& point)
	{
		return ::calcSquaredLen(a, point) < ::calcSquaredLen(b, point) ? a : b;
	}

	Point2& getEndCloserTo(const Segment2& segment)
	{
		double distA = distanceToLine(segment.a, *this),
		       distB = distanceToLine(segment.b, *this);
		return distA > distB ? a : b;
	}
};

#include <cmath>

template <typename T> constexpr T abs(T x) { return x > 0 ? x : -x; }

/*constexpr */ inline bool Point2::insideSegment(Segment2 const& line) const
{
	// http://stackoverflow.com/a/17590923
	double AB = calcSquaredLen(line.a, line.b),
	       AP = calcSquaredLen(line.a, *this),
	       PB = calcSquaredLen(line.b, *this);
	double res = std::sqrt(AP) + std::sqrt(PB) - std::sqrt(AB);
	return abs(res) < 1e-6;
}

// The distance from a point to a line defined by 2 points (i.e. a segment)
inline double distanceToLine(Point2 point, Segment2 line)
{
	return fabs((line.b.y - line.a.y) * point.x -
	            (line.b.x - line.a.x) * point.y + line.b.x * line.a.y -
	            line.b.y * line.a.x) /
	       sqrt(pow(line.b.y - line.a.y, 2) + pow(line.b.x - line.a.x, 2));
}

inline constexpr Point2 middleOf(Point2 a, Point2 b)
{
	return {a.x + (b.x - a.x) / 2, a.y + (b.y - a.y) / 2};
}

inline constexpr Point2 middleOf(Segment2 line)
{
	return middleOf(line.a, line.b);
}

struct Wall2
{
	Segment2 segment;
	Wall2* opposite = nullptr;
	bool disable_intersect_check = false;
	operator Segment2&() { return segment; }
};

struct WidePoint2
{
	Point2 point;
	float width;
	bool operator==(const WidePoint2& other) const
	{
		return point == other.point && width == other.width;
	}
};

struct CrossRoad
{
	std::vector<WidePoint2> points;

	bool operator==(CrossRoad other) { return (other.points == this->points); }
};

inline WidePoint2 widePointFromSimplePoints(Point2 a, Point2 b)
{
	Point2 mid = middleOf(a, b);
	float WIDEPOINT_WIDTH = 3;
	return {mid, WIDEPOINT_WIDTH};
}

struct WideRoad2
{
	WidePoint2 a, b;

	WideRoad2() = default;
	WideRoad2(Segment2 upper, Segment2 lower)
	    : a(widePointFromSimplePoints(upper.a, lower.a)),
	      b(widePointFromSimplePoints(upper.b, lower.b))
	{
	}

	inline bool operator==(const WideRoad2& other) const
	{
		return (a == other.a && b == other.b) || (a == other.b && b == other.a);
	}

	// NOT const - we may want to change the refference later
	const WidePoint2& getEndCloserTo(Point2 point) const
	{
		return ::calcSquaredLen(a.point, point) <
		               ::calcSquaredLen(b.point, point)
		           ? a
		           : b;
	}

	const WidePoint2& getEndCloserTo(const Segment2& segment) const
	{
		double distA = distanceToLine(a.point, segment),
		       distB = distanceToLine(b.point, segment);
		return distA < distB ? a : b;
	}

	Segment2 getSegmnet2() const
	{
		return {a.point, b.point, {0.7f, 0.7f, 0.7f, "white"}};
	}
};

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

#endif /* end of include guard: GEOMETRY_H_NTEYZYI7 */
