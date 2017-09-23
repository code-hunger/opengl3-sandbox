#ifndef SHIPCORE_H_XIP1FQRY
#define SHIPCORE_H_XIP1FQRY

#include "math/Point.h"

#include <cmath>

static float adjustSpeed(float speed, float targetSpeed, float deltaTime);

enum Direction { NONE = 0, LEFT = 1, RIGHT = -1 };

class ShipCore
{
	friend class ShipsCollection;

	math::Point2 position;
	float rotation = 0, speed = 0;

	typedef decltype(math::Point2::x) coord_t;

	ShipCore(const ShipCore&) = delete;

	void update(double deltaTime)
	{
		constexpr long double PI = 3.141592653589793238462643383279502884L;
		constexpr float PIf = static_cast<float>(PI);
		// @TODO: Two loops just for this?? Do it better.
		while (rotation > 2 * PI)
			rotation -= 2 * PIf;
		while (rotation < 0)
			rotation += 2 * PIf;

		rotation += static_cast<float>(request.direction) * 4.6f * deltaTime;
		speed = adjustSpeed(speed, request.speed, static_cast<float>(deltaTime));

		position.x += cosf(rotation) * speed * deltaTime;
		position.y += sinf(rotation) * speed * deltaTime;
	}

public:
	ShipCore(math::Point2 position) : position(position) {}

	struct
	{
		Direction direction = NONE;
		float speed = 0;
	} request;

	coord_t getX() const { return position.x; }
	coord_t getY() const { return position.y; }
	auto getPosition() const { return position; }

	float getRotation() const { return rotation; }
	float getSpeed() const { return speed; }

	ShipCore(ShipCore&&) = default;
};

static float adjustSpeed(float speed, float targetSpeed, float deltaTime)
{
	if (speed > targetSpeed) {
		float newSpeed = speed - 50 * deltaTime;
		if (newSpeed < targetSpeed) newSpeed = targetSpeed;
		return newSpeed;
	}
	if (speed < targetSpeed) {
		float newSpeed = speed + 20 * deltaTime;
		if (newSpeed > targetSpeed) newSpeed = targetSpeed;
		return newSpeed;
	}
	return speed;
}

#endif /* end of include guard: SHIPCORE_H_XIP1FQRY */
