#ifndef SHIPCORE_H_XIP1FQRY
#define SHIPCORE_H_XIP1FQRY

#include "math/Point.h"

#include <cmath>

static float adjustSpeed(float speed, float targetSpeed, float deltaTime);

enum Direction { NONE = 0, LEFT = 1, RIGHT = -1 };

class ShipCore
{
	friend class ShipsCollection;

	typedef math::Point2 Position;
	typedef decltype(Position::x) coord_t;

	Position position;
	float rotation = 0, speed = 0;

	ShipCore(const ShipCore&) = delete;

	void update(float deltaTime);

public:
	ShipCore(Position position) : position(position) {}
	ShipCore(ShipCore&& old)
	    : position(old.position), rotation(old.rotation), speed(old.speed)
	{
		old.position = {};
		old.rotation = 0;
		old.speed = 0;
	};

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
};

inline void ShipCore::update(float deltaTime)
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
