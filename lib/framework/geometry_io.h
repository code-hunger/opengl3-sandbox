#ifndef GEOMETRY_IO_H_W6BMNIRU
#define GEOMETRY_IO_H_W6BMNIRU

#ifndef GEOMETRY_H
#include "geometry.h"
#endif

#include <fstream>

std::istream &operator>>(std::istream &in, Point2 &point)
{
	return in >> point.x >> point.y;
}

std::istream &operator>>(std::istream &in, Line2 &line)
{
	return in >> line.a >> line.b;
}

std::istream &operator>>(std::istream &in, WideRoad2 &line)
{
	return in >> line.a >> line.width_a >> line.b >> line.width_b;
}

#endif /* end of include guard: GEOMETRY_IO_H_W6BMNIRU */
