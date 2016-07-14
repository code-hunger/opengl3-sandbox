#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "VertexArray.h"
#include "geometry.h"

#include <unordered_set>

typedef std::unordered_set<WideRoad2, Hash> Ways;
typedef std::unordered_set<Line2, Hash> Lines;

struct Maze
{
	Ways paths;
	Lines walls;
	VertexArray vertArray;

	void draw(GLenum mode);
	static Maze fromPaths(Ways paths);
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
