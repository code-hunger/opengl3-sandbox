#ifndef GEOMETRY_H_NTEYZYI7
#define GEOMETRY_H_NTEYZYI7

#define GEOMETRY_H

#include <cstddef>
#include <vector>

struct CrossRoad;

struct Point2
{
	float x, y;
	CrossRoad* crossRoad;

	inline bool operator==(const Point2& other) const
	{
		return this->x == other.x && this->y == other.y;
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

struct Segment2
{
	Point2 a, b;

	bool operator==(const Segment2& other) const
	{
		return (a == other.a && b == other.b) || (a == other.b && b == other.a);
	}

	bool intersectsWith(Segment2 other, Point2* crossPoint) const
	{
		LineEquation eThis = getEquation(), eOther = other.getEquation();

		float crossX = (eOther.b - eThis.b) / (eThis.a - eOther.a),
		      crossY = eThis.a * crossX + eThis.b;

		crossPoint->x = crossX;
		crossPoint->y = crossY;

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

	float calcSquaredLen() const
	{
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
	}

	// Returns the coefficients of the equation y = a*x + b in the form of a
	// Point2(a, b)
	LineEquation getEquation() const
	{
		float a = (this->a.y - b.y) / (this->a.x - b.x);
		return {a, this->a.y - a * this->a.x};
	}

	Point2& getEndCloserTo(Point2 point)
	{
		if (Segment2{a, point}.calcSquaredLen() <
		    Segment2{b, point}.calcSquaredLen()) {
			return a;
		}
		return b;
	}
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
	std::vector<Segment2*> lines;
	std::vector<WidePoint2> points;
};

struct WideRoad2
{
	WidePoint2 a, b;

	inline bool operator==(const WideRoad2& other) const
	{
		return (a == other.a && b == other.b) || (a == other.b && b == other.a);
	}

	constexpr Segment2 getSegmnet2() const
	{
		return Segment2{a.point, b.point};
	}
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
