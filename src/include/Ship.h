#ifndef SHIP_H_IKJGK1NW
#define SHIP_H_IKJGK1NW

#include "ShipCore.h"
#include "math/Point.h"

#include <memory>

typedef unsigned short ushort;

struct pilot_base;
struct State;

class Ship
{
	ShipCore core;

	ushort gear = 0;

	std::shared_ptr<pilot_base> pilot;

	friend class ShipsCollection;

	typedef decltype(core.getX()) coord_t;
public:
	static constexpr ushort MAX_GEAR = 3, GEAR_TO_SPEED = 10,
	                        MAX_SPEED = MAX_GEAR * GEAR_TO_SPEED;

	Ship(ShipCore core);
	Ship(Ship&&);

	coord_t getX() const { return core.getX(); }
	coord_t getY() const { return core.getY(); }
	const ShipCore& getCore() const { return core; }


	void startMoving(ushort gear = MAX_GEAR)
	{
		this->gear = gear <= MAX_GEAR ? gear : MAX_GEAR;
	}
	void stopMoving() { gear = 0; }
	void rotate(Direction direction) { core.request.direction = direction; }

	void update(const State&, float time);

	ushort getGear() const { return gear; }
	auto getDirection() const { return core.getRotation(); }
	auto getPosition() const { return core.getPosition(); }

	void setPilot(decltype(pilot));
	auto& getPilot() { return pilot; }

	~Ship();
};

math::Point2 randomPoint();

#endif /* end of include guard: SHIP_H_IKJGK1NW */
