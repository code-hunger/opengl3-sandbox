#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "graphics/VertexArray.h"
#include "graphics/geometry.h"

#include <unordered_set>
#include <vector>

// @TODO: USE forward_list
#include <list>

typedef std::unordered_set<WideRoad2, Hash> Ways;
typedef std::list<Wall2> Walls;

struct Maze
{
	Ways paths;
	Walls walls;
	VertexArray vertArray;

	void draw(GLenum mode);
	static Maze fromPaths(Ways paths);
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
