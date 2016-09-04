#ifndef GEOMETRY_IO_H_W6BMNIRU
#define GEOMETRY_IO_H_W6BMNIRU

#include <graphics/geometry.h>

#include <fstream>

inline std::istream& operator>>(std::istream& in, Point2& point)
{
	return in >> point.x >> point.y;
}

inline std::ostream& operator<<(std::ostream& out, const Point2& point)
{
	return out << point.x << ' ' << point.y;
}

inline std::istream& operator>>(std::istream& in, Segment2& line)
{
	return in >> line.a >> line.b;
}

inline std::ostream& operator<<(std::ostream& out, const Segment2& line)
{
	return out << line.color.name << ' ' << line.a << ' ' << line.b;
}

inline std::ostream& operator<<(std::ostream& out, const WidePoint2& point)
{
	return out << point.point << ' ' << point.width;
}

inline std::ostream& operator<<(std::ostream& out, const WideRoad2& line)
{
	return out << line.a << ' ' << line.b;
}

inline std::istream& operator>>(std::istream& in, WidePoint2& point)
{
	return in >> point.point >> point.width;
}

inline std::istream& operator>>(std::istream& in, WideRoad2& way)
{
	return in >> way.a >> way.b;
}

#endif /* end of include guard: GEOMETRY_IO_H_W6BMNIRU */
