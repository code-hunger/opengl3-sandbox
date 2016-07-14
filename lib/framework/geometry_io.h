#ifndef GEOMETRY_IO_H_W6BMNIRU
#define GEOMETRY_IO_H_W6BMNIRU

#ifndef GEOMETRY_H
#include "geometry.h"
#endif

#include <fstream>
#include <iostream>

using std::cout;

std::istream& operator>>(std::istream& in, Point2& point)
{
	return in >> point.x >> point.y;
}

std::ostream& operator<<(std::ostream& out, const Point2& point)
{
	return out << point.x << ' ' << point.y;
}

std::istream& operator>>(std::istream& in, Segment2& line)
{
	return in >> line.a >> line.b;
}

std::ostream& operator<<(std::ostream& out, const Segment2& line)
{
	return out << line.a << ' ' << line.b;
}

std::istream& operator>>(std::istream& in, WideRoad2& way)
{
	return in >> way.line.a >> way.width_a >> way.line.b >> way.width_b;
}

#endif /* end of include guard: GEOMETRY_IO_H_W6BMNIRU */
