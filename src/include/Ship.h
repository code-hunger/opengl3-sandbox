#ifndef SHIP_H_IKJGK1NW
#define SHIP_H_IKJGK1NW

#include "math/Point.h"
#include <memory>

typedef unsigned short ushort;

enum Rotation { NONE = 0, LEFT = 1, RIGHT = -1 };

struct pilot_base;
struct State;

class Ship
{
	math::Point2 position;
	float direction;

	float speed = 0;
	ushort gear = 0;
	Rotation rotation = NONE;

	std::shared_ptr<pilot_base> pilot;

	friend class ShipsCollection;

	Ship(math::Point2 position, float direction = 0);

public:
	static constexpr ushort MAX_GEAR = 3, GEAR_TO_SPEED = 10,
	                        MAX_SPEED = MAX_GEAR * GEAR_TO_SPEED;
	Ship(Ship&&);

	// instead of const getters, for ease of use
	const float &x = position.x, &y = position.y;

	void startMoving(ushort gear = MAX_GEAR)
	{
		this->gear = gear <= MAX_GEAR ? gear : MAX_GEAR;
	}
	void stopMoving() { gear = 0; }
	void rotate(Rotation rotation) { this->rotation = rotation; }

	void update(const State&, float time);

	auto getDirection() const { return direction; }
	auto getPosition() const { return position; }

	void setPilot(decltype(pilot));
	auto& getPilot() { return pilot; }

	~Ship();
};

math::Point2 randomPoint();

#endif /* end of include guard: SHIP_H_IKJGK1NW */
