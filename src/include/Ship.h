#ifndef SHIP_H_IKJGK1NW
#define SHIP_H_IKJGK1NW

#include "graphics/ShaderProgram.h"
#include "graphics/State.h"
#include "graphics/VertexArray.h"
#include "math/types.h"

class Ship
{
	math::Point2 position;
	float direction;

public:
	// instead of const getters, for ease of use
	const float &x = position.x, &y = position.y;

	Ship(math::Point2 position, float direction = 0);

	Ship(Ship&& other) : position(other.position), direction(other.direction) {}

	void update(const State&, float time);

	auto getDirection() const { return direction; }
};

math::Point2 randomPoint();

#endif /* end of include guard: SHIP_H_IKJGK1NW */
