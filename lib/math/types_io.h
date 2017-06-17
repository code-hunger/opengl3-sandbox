#ifndef TYPES_IO_H_W6BMNIRU
#define TYPES_IO_H_W6BMNIRU

#include "types.h"

#include <fstream>
#include <iomanip> // std::setw

namespace math {

using std::istream;
using std::ostream;

inline istream& operator>>(istream& in, Point2& point)
{
	return in >> point.x >> point.y;
}

inline ostream& operator<<(ostream& out, const Point2& point)
{
	using namespace std;
	return out << setprecision(2) << setw(5) << right << point.x << ' '
	           << setprecision(2) << setw(5) << left << point.y;
}

inline ostream& operator<<(ostream& out, const ColorSegment2& line)
{
	return out << std::setw(10) << line.color.name << line.line.a
	           << line.line.b;
}

inline ostream& operator<<(ostream& out, const WidePoint2& point)
{
	return out << point.point << ' ' << point.width;
}

inline ostream& operator<<(ostream& out, const WideRoad2& line)
{
	return out << line.a << ' ' << line.b;
}

inline istream& operator>>(istream& in, WidePoint2& point)
{
	return in >> point.point >> point.width;
}

inline istream& operator>>(istream& in, WideRoad2& way)
{
	return in >> way.a >> way.b;
}
}

#endif /* end of include guard: TYPES_IO_H_W6BMNIRU */
