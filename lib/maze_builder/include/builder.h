#ifndef BUILDER_H_6BSAEIFN
#define BUILDER_H_6BSAEIFN

#include "math/include/geometry.h"

// @TODO: USE forward_list
#include <list>

typedef std::list<WideRoad2> Ways;

struct Builder
{
	bool join = true;
	unsigned max_count = 0;
	void build_from_paths(const Ways&, std::list<Segment2>&);
};

#endif /* end of include guard: BUILDER_H_6BSAEIFN */
