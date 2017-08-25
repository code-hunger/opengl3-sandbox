#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "graphics/ShaderProgram.h"
#include "graphics/VertexArray.h"
#include "math/types.h"

class Maze
{
public:
	static Maze build(math::WideRoads&&, const math::ColorSegmentList&);

	Maze(Maze&&) = default;

	void draw(uint mode);

private:
	math::WideRoads paths;
	math::ColorSegmentList walls;

	VertexArray vertArray;
	ShaderProgram shaderProgram{"maze"};

	Maze(const math::WideRoads&, const math::ColorSegmentList&, VertexArray&&);
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
