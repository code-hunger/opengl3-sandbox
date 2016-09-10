#ifndef DEF_H_QMS9RCZ0
#define DEF_H_QMS9RCZ0

#include "graphics/geometry_io.h"

#include <list>
#include <set>
#include <unordered_set>
#include <vector>

#include <iostream>
using std::cout;
using std::endl;

typedef std::unordered_set<WideRoad2, Hash> Ways;
typedef std::list<Wall2> Walls;
typedef std::vector<Color> Colors;
typedef std::list<CrossRoad> CrossRoads;

/* Have fun! C++ rocks!
 *
 * Unless being #defined, it has to be an inline function in order to be in the
 * header. But then we have to use auto in function argument, which is not
 * allowed (in clang, at least) unless it's a lambda. Third option is to use
 * templates, which don't allow inline functions and therefore it can't stay in
 * the header
 */
#define get_ptr_val(PTR)                                                       \
	[](const auto& ptr) {                                                      \
		char addr[20];                                                         \
		sprintf(addr, "%llu", &ptr);                                           \
		return atoll(addr) % 10000;                                            \
	}(PTR)

// It's wrong. To be removed
// @Deprecated
inline void dump(Walls walls)
{
	std::set<CrossRoad*> count_unique;
	for (const auto& _p : walls) {
		auto p = _p.segment;

		count_unique.insert(p.a.crossRoad);
		count_unique.insert(p.b.crossRoad);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
		cout << p.color.name << endl
		     << p.a << "(" << get_ptr_val(*p.a.crossRoad) << ") : ";
		for (const auto& c : p.a.crossRoad->points) {
			cout << c << ", ";
		}
		cout << endl << p.b << "(" << get_ptr_val(*p.b.crossRoad) << ") : ";
#pragma GCC diagnostic pop
		for (const auto& c : p.b.crossRoad->points) {
			cout << c << ", ";
		}
		cout << endl;
	}
	cout << "Unique crossRoads: " << count_unique.size() << endl;
}

#endif /* end of include guard: DEF_H_QMS9RCZ0 */
