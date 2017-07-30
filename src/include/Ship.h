#ifndef SHIP_H_IKJGK1NW
#define SHIP_H_IKJGK1NW

#include "graphics/State.h"
#include "math/types.h"
#include <memory>

enum Rotation { NONE, LEFT, RIGHT };

struct pilot_base;

class Ship
{
	math::Point2 position;
	float direction;

	float speed = 0;
	ushort gear = 0;
	Rotation rotation = NONE;

	std::unique_ptr<pilot_base> pilot;

	friend class ShipsCollection;

	Ship(math::Point2 position, float direction = 0);

public:
	Ship(Ship&&);

	// instead of const getters, for ease of use
	const float &x = position.x, &y = position.y;

	void startMoving(ushort gear = 3) { this->gear = gear <= 3 ? gear : 3u; }
	void stopMoving() { gear = 0; }
	void rotate(Rotation rotation) { this->rotation = rotation; }

	void update(const State&, float time);

	auto getDirection() const { return direction; }
	auto getPosition() const { return position; }

	~Ship();
};

math::Point2 randomPoint();

#endif /* end of include guard: SHIP_H_IKJGK1NW */
