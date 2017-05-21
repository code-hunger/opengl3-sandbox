#ifndef SHIP_H_IKJGK1NW
#define SHIP_H_IKJGK1NW

#include "graphics/include/ShaderProgram.h"
#include "graphics/include/VertexArray.h"
#include "math/include/types.h"
#include "graphics/include/State.h"

class Ship
{
public:
	Ship(math::Point2 position, double direction = 0);

	Ship(Ship&&) = default;

	void update(const State&, double time);

private:
	math::Point2 position;
	double direction;
};

#endif /* end of include guard: SHIP_H_IKJGK1NW */
