#ifndef BUILDER_H_6BSAEIFN
#define BUILDER_H_6BSAEIFN

#include "math/include/types.h"

struct Builder
{
	bool join = true;
	unsigned max_count = 0;

	math::ColorSegmentList build_from_paths(math::WideRoads&);
};

#endif /* end of include guard: BUILDER_H_6BSAEIFN */
