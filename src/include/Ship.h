#ifndef SHIP_H_IKJGK1NW
#define SHIP_H_IKJGK1NW

#include "graphics/ShaderProgram.h"
#include "graphics/State.h"
#include "graphics/VertexArray.h"
#include "math/types.h"

class Ship
{
public:
	Ship(math::Point2 position, float direction = 0);

	Ship(Ship&&) = default;

	void update(const State&, float time);

	auto getX() const { return position.x; }
	auto getY() const { return position.y; }

	auto getDirection() const { return direction; }

private:
	math::Point2 position;
	float direction;
};

math::Point2 randomPoint();

#endif /* end of include guard: SHIP_H_IKJGK1NW */
