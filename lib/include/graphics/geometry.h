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
};

struct Point2
{
	float x, y;
	CrossRoad* crossRoad = nullptr;

	inline bool operator==(const Point2& other) const
	{
		return this->x == other.x && this->y == other.y;
	}

	operator bool () = delete;

	// Don't allow this - *crossRoad must be preserved
	void operator=(const Point2&) = delete;

	// This only moves coordinates, doesn't copy *crossRoad
	Point2& moveTo(const Point2& other)
	{
		this->x = other.x;
		this->y = other.y;
		return *this;
	}
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

struct Segment2
{
	Point2 a, b;
	Color color{};

	bool operator==(const Segment2& other) const
	{
		return (a == other.a && b == other.b) || (a == other.b && b == other.a);
	}

	bool intersectsWith(Segment2 other, Point2* crossPoint) const
	{
		LineEquation eThis = getEquation(), eOther = other.getEquation();

		float crossX = (eOther.b - eThis.b) / (eThis.a - eOther.a),
		      crossY = eThis.a * crossX + eThis.b;

		if (crossPoint != nullptr) {
			crossPoint->x = crossX;
			crossPoint->y = crossY;
		}

		if ((crossX < b.x && crossX > a.x) || (crossX > b.x && crossX < a.x)) {
			// this crossX yes
		} else {
			return false;
		}
		if ((crossX < other.b.x && crossX > other.a.x) ||
		    (crossX > other.b.x && crossX < other.a.x)) {
			// other crossX yes
		} else {
			return false;
		}
		if ((crossY < b.y && crossY > a.y) || (crossY > b.y && crossY < a.y)) {
			// this crossY yes
		} else {
			return false;
		}
		if ((crossY < other.b.y && crossY > other.a.y) ||
		    (crossY > other.b.y && crossY < other.a.y)) {
			// other crossY yes
		} else {
			return false;
		}
		return true;
	}

	constexpr float calcSquaredLen() const { return ::calcSquaredLen(a, b); }

	LineEquation getEquation() const
	{
		float a = (this->a.y - b.y) / (this->a.x - b.x);
		return {a, this->a.y - a * this->a.x};
	}

	// NOT const - we may want to change the refference later
	Point2& getEndCloserTo(Point2 point)
	{
		return ::calcSquaredLen(a, point) < ::calcSquaredLen(b, point) ? a : b;
	}
};

inline constexpr Point2 middleOf(Segment2 line)
{
	return {line.a.x + (line.a.x - line.b.x) / 2,
	        line.a.y + (line.a.y - line.b.y) / 2};
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
	short unsigned width;
	bool operator==(const WidePoint2& other) const
	{
		return point == other.point && width == other.width;
	}
};

struct CrossRoad
{
	std::vector<WidePoint2> points;

	bool operator== (CrossRoad other) {
		return (other.points == this->points);
	}
};

struct WideRoad2
{
	WidePoint2 a, b;

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

	constexpr Segment2 getSegmnet2() const { return {a.point, b.point}; }
};

struct Hash
{
	size_t operator()(const WideRoad2& way) const
	{
		return static_cast<size_t>(
		    static_cast<float>(way.a.width * way.b.width) +
		    way.getSegmnet2().calcSquaredLen());
	}

	size_t operator()(const Segment2& line) const
	{
		return static_cast<size_t>(10000 * line.a.x * line.a.y +
		                           line.calcSquaredLen());
	}
};

#endif /* end of include guard: GEOMETRY_H_NTEYZYI7 */
