#include "Ship.h"
#include "logger/logger.h"
#include "pilots/line_formation.cpp"

#include <stdlib.h>

Ship::Ship(math::Point2 position, float direction)
    : position(position), direction(direction), pilot{}
{
}

Ship::Ship(Ship&& other)
    : position(other.position), direction(other.direction), speed(other.speed),
      rotation(other.rotation), pilot(std::move(other.pilot))
{
}

void Ship::update(const State&, float deltaTime)
{
	if (rotation) {
		direction += deltaTime * 4.6f * rotation;
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

Ship::~Ship() = default;
