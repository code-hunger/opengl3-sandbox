#ifndef SHIP_H_IKJGK1NW
#define SHIP_H_IKJGK1NW

#include "graphics/ShaderProgram.h"
#include "graphics/State.h"
#include "graphics/VertexArray.h"
#include "math/types.h"

enum Rotation { NONE, LEFT, RIGHT };

class Ship
{
	math::Point2 position;
	float direction;

	ushort speed = 0;
	Rotation rotation = NONE;

	friend class ShipsCollection;

	Ship(math::Point2 position, float direction = 0);

public:
	Ship(Ship&& other)
	    : position(other.position), direction(other.direction),
	      speed(other.speed), rotation(other.rotation)
	{
	}

	// instead of const getters, for ease of use
	const float &x = position.x, &y = position.y;

	void startMoving() { speed = 20; }
	void stopMoving() { speed = 0; }
	void rotate(Rotation rotation) { this->rotation = rotation; }

	void update(const State&, float time);

	auto getDirection() const { return direction; }
};

math::Point2 randomPoint();

#endif /* end of include guard: SHIP_H_IKJGK1NW */
