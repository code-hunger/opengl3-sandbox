#include "Ship.h"

#include <GLFW/glfw3.h>

Ship::Ship(math::Point2 position, double direction)
    : position(position), direction(direction)
{
}

void Ship::update(const State& state, double deltaTime)
{
	if (state.keys[GLFW_KEY_LEFT]) {
		direction += deltaTime * 3.6;
	}
	if (state.keys[GLFW_KEY_RIGHT]) {
		direction -= deltaTime * 3.6;
	}
	if (state.keys[GLFW_KEY_SPACE]) {
		position.x += cos(direction) * 20 * deltaTime;
		position.y += sin(direction) * 20 * deltaTime;
	}
}
