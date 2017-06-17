#ifndef TYPES_H_H3EJYX5K
#define TYPES_H_H3EJYX5K

#include <cmath>

// if we can't check for header existence, or <optional> is available,
// then use optional (this happens in VS2017)
#if !defined(__has_include) || __has_include(<optional>)
#include <optional>
template <typename T> using optional = std::optional<T>;

// We can check for header existence and <optional> is not available.
// Try with <experimental/optional>! (happens in GCC)
#elif __has_include(<experimental/optional>)
#include <experimental/optional>
template <typename T> using optional = std::experimental::optional<T>;

// if we can check for header existence and neither <optional>
// nor <experimental/optional> is available, abort compilation
#else
#error "Neither header <optional> nor <experimental/optional> available!"
#endif

#include <list>
#include <vector>

namespace math {

struct Color
{
	const float r, g, b;
	char const* const name;
	operator const char*() const { return name; }

	static const Color& next();
};

struct Segment2;

struct Point2
{
	float x, y;

	bool operator==(const Point2& other) const
	{
		return this->x == other.x && this->y == other.y;
	}

	constexpr inline float distance2(const Point2&) const;
	constexpr inline float distance2(const Segment2&) const;
};

struct LineEquation
{
	float a, b;
};

struct ColorSegment2;

struct Segment2
{
	const Point2 a, b;
	const float slope = atanf((b.y - a.y) / (b.x - a.x));
	bool operator==(const Segment2& other) const
	{
		return (a == other.a && b == other.b) || (a == other.b && b == other.a);
	}

	inline constexpr float len2() const;
	inline constexpr LineEquation equation() const;
	inline constexpr bool isInside(const Point2&) const;
	inline const Point2& getEndCloserTo(const Point2&) const;
	inline ColorSegment2 colorSegment(Color color = Color::next()) const;
	inline ColorSegment2 whiteSegment() const;
};

struct ColorSegment2
{
	const Segment2 line;
	Color color;

	operator Segment2() const { return line; }
};

struct WidePoint2;
struct WideRoad2;

struct CrossRoad
{
	using Points = std::vector<std::pair<WidePoint2*, WideRoad2*>>;
	Points points{};
};
typedef std::list<CrossRoad> CrossRoads;

struct WidePoint2
{
	Point2 point;
	float width;
	optional<CrossRoads::iterator> crossRoad{};

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

	inline float len2() const;
	inline float widthAt(const Point2&) const;
	inline WidePoint2& getEndCloserTo(const Point2&);

	inline ColorSegment2 colorSegment(Color color = Color::next()) const;
	inline ColorSegment2 whiteSegment() const;
};

typedef std::pair<ColorSegment2, ColorSegment2> ColorSegment2Pair;
typedef std::list<math::ColorSegment2> ColorSegmentList;
typedef std::list<WideRoad2> WideRoads;
}

#endif /* end of include guard: TYPES_H_H3EJYX5K */
