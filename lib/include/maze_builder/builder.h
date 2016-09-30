#ifndef BUILDER_H_6BSAEIFN
#define BUILDER_H_6BSAEIFN

#include <unordered_set>
#include "graphics/geometry.h"

// @TODO: USE forward_list
#include <list>

typedef std::unordered_set<WideRoad2, Hash> Ways;

void build_from_paths(const Ways&, std::list<Segment2> &);

#endif /* end of include guard: BUILDER_H_6BSAEIFN */
