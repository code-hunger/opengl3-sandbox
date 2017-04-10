#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "graphics/include/VertexArray.h"
#include "math/include/types.h"

#include <unordered_set>

// @TODO: USE forward_list
#include <list>

namespace math {
typedef std::list<math::ColorSegment2> ColorSegmentList;
}

class Maze
{
public:
	static Maze build(math::WideRoads&&, const math::ColorSegmentList&);

	void draw(GLenum mode);

private:
	math::WideRoads paths;
	math::ColorSegmentList walls;
	VertexArray vertArray;

	Maze(math::WideRoads, math::ColorSegmentList, VertexArray);
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
