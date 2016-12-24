#ifndef BUILDER_H_6BSAEIFN
#define BUILDER_H_6BSAEIFN

#include "math/include/geometry.h"

// @TODO: USE forward_list
#include <list>

typedef std::list<WideRoad2> Ways;
typedef std::list<CrossRoad> CrossRoads;
typedef std::list<std::pair<std::pair<Segment2, Segment2>, WideRoad2>> PWalls;

class Builder
{
private:
	std::pair<Segment2, Segment2> createWalls(WideRoad2 const&);

	void add_a_single_way_to_maze(PWalls& wallsP, WideRoad2 const& way,
	                              CrossRoads& cross_roads);

	void normalizeWays(Ways&);

public:
	bool join = true;
	unsigned max_count = 0;

	std::list<Segment2> build_from_paths(Ways&);
};

#endif /* end of include guard: BUILDER_H_6BSAEIFN */
