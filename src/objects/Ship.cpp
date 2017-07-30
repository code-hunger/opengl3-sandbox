#include "Ship.h"
#include "logger/logger.h"
#include <stdlib.h>

Ship::Ship(math::Point2 position, float direction)
    : position(position), direction(direction)
{
}

void Ship::update(const State&, float deltaTime)
{
	if (rotation) {
		direction += deltaTime * 4.6f * (rotation == LEFT ? 1 : -1);
	}

	ushort targetSpeed = static_cast<ushort>(gear * 10);
	if (speed > targetSpeed) {
		speed -= 50 * deltaTime;
		if (speed < targetSpeed) speed = targetSpeed;
	}
	if (speed < targetSpeed) {
		speed += 20 * deltaTime;
		if (speed > targetSpeed) speed = targetSpeed;
	}

	position.x += cosf(direction) * speed * deltaTime;
	position.y += sinf(direction) * speed * deltaTime;
}

math::Point2 randomPoint()
{
	math::Point2 p{static_cast<float>(rand() / RAND_MAX) * 100,
	               static_cast<float>(rand() / RAND_MAX) * 100};
	LOG << p.x << ' ' << p.y << ' ' << rand() / RAND_MAX;
	return p;
}
