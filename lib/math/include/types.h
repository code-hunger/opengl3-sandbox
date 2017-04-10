#ifndef TYPES_H_H3EJYX5K
#define TYPES_H_H3EJYX5K

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <deque>
#include <list>
#include <vector>

namespace math {

struct Color
{
	const float r, g, b;
	char const* const name;
	operator const char*() const { return name; }

	static const Color& next()
	{
		static const std::vector<Color> colors = {
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

struct Point2
{
	float x, y;

	bool operator==(const Point2& other) const
	{
		return this->x == other.x && this->y == other.y;
	}
};

struct Segment2
{
	const Point2 a, b;
	const double slope = atan((b.y - a.y) / (b.x - a.x));
	bool operator==(const Segment2& other) const
	{
		return (a == other.a && b == other.b) || (a == other.b && b == other.a);
	}
};

struct LineEquation
{
	float a, b;
};

struct ColorSegment2
{
	const Segment2 line;
	Color color;

	operator Segment2() const { return line; }
};

struct CrossRoad;

struct WidePoint2
{
	Point2 point;
	float width;
	CrossRoad* crossRoad = nullptr;

	bool operator==(const WidePoint2& other) const
	{
		return point == other.point && width == other.width;
	}
};

struct WideRoad2
{
	WidePoint2 a, b;

	bool operator==(const WideRoad2& other) const
	{
		return (a == other.a && b == other.b) || (a == other.b && b == other.a);
	}
	operator Segment2() const { return {a.point, b.point}; }
	Segment2 segment() const { return {a.point, b.point}; }
};

struct CrossRoad
{
	std::vector<Point2*> points{};
};

typedef std::pair<ColorSegment2, ColorSegment2> ColorSegment2Pair;
typedef std::list<WideRoad2> WideRoads;
typedef std::list<CrossRoad> CrossRoads;
}

#endif /* end of include guard: TYPES_H_H3EJYX5K */
