#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "graphics/include/VertexArray.h"
#include "math/include/geometry.h"
#include "maze_builder/include/builder.h"

#include <unordered_set>

// @TODO: USE forward_list
#include <list>

typedef std::list<Segment2> Walls;

class Maze
{
public:
	static Maze build(const Ways&, Builder&& = {});

	void draw(GLenum mode);

private:
	Ways paths;
	Walls walls;
	VertexArray vertArray;

	Maze(Ways, Walls, VertexArray);
	static VertexArray wallsToVertArr(const Walls&);
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
