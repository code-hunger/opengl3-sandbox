#include "pilot_base.h"

#include "Ship.h"

#include "logger/logger.h"
#include <cassert>
#include <cmath>

auto determine_rotation(double sinCurrent, double sinTarget, double cosCurrent,
                        double cosTarget)
{
	double diff = (sinCurrent > 0 ? -cosTarget : cosTarget) +
	              (sinTarget > 0 ? cosCurrent : -cosCurrent);

	// Negative diff means turn left, positive means turn right
	return diff > 1e-5 ? LEFT : diff < -1e-5 ? RIGHT : NONE;
}

auto determine_rotation(const Ship& source, const math::Point2& destination,
                        double currentDistance2 = 0)
{
	if (currentDistance2 == 0) {
		currentDistance2 = source.getPosition().distance2(destination);
	}
	const double currentDistance = sqrt(currentDistance2),
	             currentDirection = source.getDirection(),

	             cosTarget = (destination.x - source.x) / currentDistance,
	             sinTarget = (destination.y - source.y) / currentDistance,

	             cosCurrent = cos(currentDirection),
	             sinCurrent = sin(currentDirection);

	return determine_rotation(sinCurrent, sinTarget, cosCurrent, cosTarget);
}

namespace pilots {

void follower::operator()(Ship& ship)
{
	double currentDistance2 =
	    ship.getPosition().distance2(leader->getPosition());

	Rotation rotation =
	    determine_rotation(ship, leader->getPosition(), currentDistance2);
	ship.rotate(rotation);

	if (currentDistance2 < distance * distance) {
		ship.stopMoving();
	} else {
		ship.startMoving(1);
	}
}
}
