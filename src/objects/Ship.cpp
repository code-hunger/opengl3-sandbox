#include "Ship.h"

#include "pilots/pilot_base.h"
#include "math/types.h"
#include "logger/logger.h"

#include <cstdlib>

Ship::Ship(ShipCore core) : core(std::move(core)), pilot{} {}

Ship::Ship(Ship&& other)
    : core(std::move(other.core)), pilot(std::move(other.pilot))
{
}

void Ship::update(const State&, float)
{
	if (pilot) pilot->operator()(*this); // Should pilot really be a unique_ptr?

	core.request.speed = static_cast<ushort>(gear * GEAR_TO_SPEED);
}

void Ship::setPilot(std::shared_ptr<pilot_base> new_pilot)
{
	pilot = new_pilot;
}

math::Point2 randomPoint()
{
	math::Point2 p{static_cast<float>(rand() / RAND_MAX) * 100,
	               static_cast<float>(rand() / RAND_MAX) * 100};
	LOG << p.x << ' ' << p.y << ' ' << rand() / RAND_MAX;
	return p;
}

Ship::~Ship() = default;
