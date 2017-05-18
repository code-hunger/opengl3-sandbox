#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "graphics/include/VertexArray.h"
#include "graphics/include/Drawable.h"
#include "math/include/types.h"

class Maze: public Drawable
{
public:
	static Maze build(math::WideRoads&&, const math::ColorSegmentList&);

	void draw(uint mode) override;

private:
	math::WideRoads paths;
	math::ColorSegmentList walls;

	VertexArray vertArray;

	Maze(const math::WideRoads&, const math::ColorSegmentList&, const VertexArray&);
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
