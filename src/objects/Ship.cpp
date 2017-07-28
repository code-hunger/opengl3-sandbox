#include "Ship.h"
#include "logger/logger.h"
#include <stdlib.h>

#include <GLFW/glfw3.h>

Ship::Ship(math::Point2 position, float direction)
    : position(position), direction(direction)
{
}

void Ship::update(const State& state, float deltaTime)
{
	if (state.keys[GLFW_KEY_LEFT]) {
		direction += deltaTime * 3.6f;
	}
	if (state.keys[GLFW_KEY_RIGHT]) {
		direction -= deltaTime * 3.6f;
	}
	if (state.keys[GLFW_KEY_SPACE]) {
		position.x += cosf(direction) * 20 * deltaTime;
		position.y += sinf(direction) * 20 * deltaTime;
	}
}

math::Point2 randomPoint()
{
	math::Point2 p{static_cast<float>(rand() / RAND_MAX) * 100,
	               static_cast<float>(rand() / RAND_MAX) * 100};
	LOG << p.x << ' ' << p.y << ' ' << rand() / RAND_MAX;
	return p;
}
