#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "graphics/include/ShaderProgram.h"
#include "graphics/include/VertexArray.h"
#include "math/include/types.h"

class Maze
{
public:
	static Maze build(math::WideRoads&&, const math::ColorSegmentList&);

	void draw(uint mode);

private:
	math::WideRoads paths;
	math::ColorSegmentList walls;

	VertexArray vertArray;
	ShaderProgram shaderProgram{};

	Maze(const math::WideRoads&, const math::ColorSegmentList&, VertexArray&&);
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
