#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "VertexArray.h"
#include "geometry.h"

#include <unordered_set>
#include <vector>

// @TODO: USE forward_list
#include <list>

typedef std::unordered_set<WideRoad2, Hash> Ways;

struct Maze
{
	Ways paths;
	std::list<Segment2> walls;
	VertexArray vertArray;

	void draw(GLenum mode);
	static Maze fromPaths(Ways paths);
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
