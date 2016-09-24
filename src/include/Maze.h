#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "graphics/VertexArray.h"
#include "graphics/geometry.h"

#include <unordered_set>

// @TODO: USE forward_list
#include <list>

typedef std::unordered_set<WideRoad2, Hash> Ways;
typedef std::list<std::pair<Segment2, Segment2>> Walls;

class Maze
{
	Ways paths;
	Walls walls;
	VertexArray vertArray;

	Maze(Ways, Walls, VertexArray);
	static VertexArray wallsToVertArr(const Walls&);
public:
	static Maze build(const Ways&, void (*)(const Ways&, Walls&));

	void draw(GLenum mode);
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
