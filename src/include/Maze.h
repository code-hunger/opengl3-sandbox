#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "graphics/ShaderProgram.h"
#include "graphics/VertexArray.h"
#include "math/types.h"

VertexArray wallsToVertArr(const math::ColorSegmentList& walls);

class Maze
{
public:
	Maze(const math::WideRoads&, const math::ColorSegmentList&);

	Maze(Maze&&) = default;

	void draw(uint mode);

private:
	math::WideRoads paths;
	math::ColorSegmentList walls;

	VertexArray vertArray = wallsToVertArr(walls);
	ShaderProgram shaderProgram{"maze"};
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
