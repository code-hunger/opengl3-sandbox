#pragma once

namespace math {
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
}
