#ifndef BUILDER_H_6BSAEIFN
#define BUILDER_H_6BSAEIFN

#include "math/include/types.h"

// @TODO: USE forward_list
#include <list>

namespace math {
typedef std::list<math::ColorSegment2> ColorSegmentList;
}

struct Builder
{
	bool join = true;
	unsigned max_count = 0;

	math::ColorSegmentList build_from_paths(math::WideRoads&);
};

#endif /* end of include guard: BUILDER_H_6BSAEIFN */
