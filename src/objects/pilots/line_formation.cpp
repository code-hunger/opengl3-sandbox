#include "pilots/pilot_base.h"

#include "math/geometry.h"
#include "math/types_io.h"
#include "Ship.h"

#include "logger/logger.h"
#include "graphics/State.h"

#include <algorithm>
#include <cassert>
#include <cmath>

auto determine_rotation(double sinCurrent, double sinTarget, double cosCurrent,
                        double cosTarget)
{
	double diff = (sinCurrent > 0 ? -cosTarget : cosTarget) +
	              (sinTarget > 0 ? cosCurrent : -cosCurrent);

	// Negative diff means turn left, positive means turn right
	return diff > 1e-3 ? LEFT : diff < -1e-3 ? RIGHT : NONE;
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

auto determine_rotation(double currentDirection, double targetDirection)
{
	return determine_rotation(sin(currentDirection), sin(targetDirection),
	                          cos(currentDirection), cos(targetDirection));
}

/**
 * The distance to the target is taken into account when deciding the speed at
 * which to move the ship.
 * @return whether the ship was moved or not
 */
bool move_toward_point(Ship& ship, math::Point2 targetPoint)
{
	double currentDistance2 = ship.getPosition().distance2(targetPoint);

	if (currentDistance2 < 1) {
		ship.stopMoving();
		return false;
	}

	Rotation rotation = determine_rotation(ship, targetPoint, currentDistance2);
	ship.rotate(rotation);

	if (rotation != NONE &&
	    currentDistance2 < ship.getGear() * ship.GEAR_TO_SPEED) {
		ship.stopMoving();
		return false;
	}

	ushort gear =
	    currentDistance2 > ship.MAX_SPEED * ship.MAX_SPEED ? ship.MAX_GEAR : 1;
	ship.startMoving(gear);

	return true;
}

namespace pilots {

void follower::operator()(Ship& ship) const
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

math::Point2 line_follower::calc_target_position(const Ship& ship) const
{
	ushort index = ships.at(&ship),
	       ships_size = static_cast<ushort>(ships.size());

	float a = distance_to_leader(static_cast<short>(index - ships_size / 2));

	constexpr ushort MAX_ON_LINE = 77;

	if (index >= MAX_ON_LINE) {
		a *= static_cast<float>(index / MAX_ON_LINE + 1);
		index %= MAX_ON_LINE;
	}

	float b = distance_between * (static_cast<float>(index) -
	                              std::min(MAX_ON_LINE, ships_size) / 2),
	      hypo = static_cast<float>(sqrt(a * a + b * b)),
	      g = leader->getDirection() - atanf(b / a);

	if (a < 0) g += PIf;

	return {leader->x - hypo * cosf(g), leader->y - hypo * sinf(g)};
}

void line_follower::operator()(Ship& ship) const
{
	math::Point2 targetPoint = calc_target_position(ship);
	bool has_moved = move_toward_point(ship, targetPoint);

	if (!has_moved)
		ship.rotate(
		    determine_rotation(ship.getDirection(), leader->getDirection()));
}

void keyboard_controlled::operator()(Ship& ship) const
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
