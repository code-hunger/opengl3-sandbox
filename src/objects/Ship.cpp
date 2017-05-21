#include "Ship.h"

Ship::Ship(math::Point2 position, double direction)
    : position(position), direction(direction)
{
}

void Ship::update(const State&, double) {}
