#include "pilot_base.h"

#include "logger/logger.h"
#include <cassert>
#include <cmath>

namespace pilots {
void follower::operator()(Ship& ship)
{
	double currentDistance2 =
	    ship.getPosition().distance2(leader->getPosition());

	if (currentDistance2 < distance * distance) {
		ship.stopMoving();
		ship.rotate(NONE);
		return;
	}

	ship.startMoving(1);

	const double currentDistance = sqrt(currentDistance2),
	             currentDirection = ship.getDirection(),

	             cosTarget = (leader->x - ship.x) / currentDistance,
	             sinTarget = (leader->y - ship.y) / currentDistance,

	             cosCurrent = cos(currentDirection),
	             sinCurrent = sin(currentDirection);

	double diff = 0;
	if (sinCurrent > 0) {
		if (sinTarget > 0) {
			diff = -cosTarget + cosCurrent;
		} else {
			diff = -cosTarget - cosCurrent;
		}
	} else {
		if (sinTarget > 0) {
			diff = cosTarget + cosCurrent;
		} else {
			diff = cosTarget - cosCurrent;
		}
	}

	// Negative diff means turn left, positive means turn right
	ship.rotate(diff > 1e-5 ? LEFT : diff < -1e-5 ? RIGHT : NONE);
}
}
