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

void line_follower::operator()(Ship& ship)
{
	float index = ships[&ship], a = distance_to_leader,
	      b = distance_between * (index - (1.0f + ships.size()) / 2.0f),
	      hypo = static_cast<float>(sqrt(a * a + b * b)),
	      g = leader->getDirection() - atanf(b / a);

	if (distance_to_leader < 0) g += static_cast<float>(PI);

	math::Point2 targetPoint{leader->x - hypo * cosf(g),
	                         leader->y - hypo * sinf(g)};

	double currentDistance2 = ship.getPosition().distance2(targetPoint);

	Rotation rotation;
	if (currentDistance2 < 1) {
		ship.stopMoving();
		rotation = determine_rotation(
		    sin(ship.getDirection()), sin(leader->getDirection()),
		    cos(ship.getDirection()), cos(leader->getDirection()));
		LOG << index << " Only rotate!";
	} else {
		ushort gear = currentDistance2 > ship.MAX_SPEED * ship.MAX_SPEED
		                  ? ship.MAX_GEAR
		                  : 1;
		ship.startMoving(gear);
		LOG << index << "Gear: " << gear;
		rotation = determine_rotation(ship, targetPoint, currentDistance2);
	}
	ship.rotate(rotation);
}

void keyboard_controlled::operator()(Ship& ship)
{
	if (state->keys[FORWARD_KEY]) {
		ship.startMoving();
	} else {
		ship.stopMoving();
	}

	if (state->keys[LEFT_KEY]) {
		ship.rotate(Rotation::LEFT);
	} else if (state->keys[RIGHT_KEY]) {
		ship.rotate(Rotation::RIGHT);
	} else {
		ship.rotate(Rotation::NONE);
	}
}
}
