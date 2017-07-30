#include "pilot_base.h"

#include "logger/logger.h"
#include <cassert>
#include <cmath>

namespace pilots {
void follower::operator()(Ship& ship)
{
	float currentDistance = static_cast<float>(
	    sqrt(ship.getPosition().distance2(leader->getPosition())));
	if (currentDistance > distance) {
		ship.startMoving(1);

		const float cosTarget = (leader->x - ship.x) / currentDistance,
		            sinTarget = (leader->y - ship.y) / currentDistance;

		float currentDirection = ship.getDirection(),
		      targetDirection = asinf(sinTarget);

		if (cosTarget < 0) {
			targetDirection = static_cast<float>(PI) - targetDirection;
		} else if (sinTarget < 0) {
			targetDirection += static_cast<float>(2 * PI);
		}

		const float cosCurrent = cosf(currentDirection),
		            sinCurrent = sinf(currentDirection);

		float diff = 0;
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

		if (diff > 1e-5)
			ship.rotate(LEFT);
		else if (diff < -1e-5)
			ship.rotate(RIGHT);
		else
			ship.rotate(NONE);
	} else {
		ship.stopMoving();
		ship.rotate(NONE);
	}
}
}
