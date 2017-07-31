#ifndef PILOT_BASE_H_SYAGZKV9
#define PILOT_BASE_H_SYAGZKV9

#include "logger/logger.h"
#include <map>

class Ship;
typedef unsigned short ushort;

struct pilot_base
{
	virtual void operator()(Ship&) = 0;
	virtual ~pilot_base() = default;
};

namespace pilots {

/**
 * Let's say this pilot leads ships in a line
 */
struct follower : pilot_base
{
	follower(const Ship& leader, ushort distance = 20)
	    : leader(&leader), distance(distance)
	{
	}

	follower(const follower&) = delete;
	void operator=(const follower&) = delete;

	void operator()(Ship&) override;

	void setLeader(const Ship& leader) { this->leader = &leader; }

private:
	const Ship* leader;
	ushort distance;
};

struct line_follower : pilot_base
{
	line_follower(const Ship& leader) : leader(&leader) {}

	line_follower(const line_follower&) = delete;
	void operator=(const line_follower&) = delete;

	void attach_ship(Ship& ship)
	{
		ships[&ship] = static_cast<ushort>(ships.size());
	}

	void operator()(Ship&) override;

private:
	const Ship* leader;
	float distance_to_leader = 20, distance_between = 15;

	std::map<Ship*, ushort> ships{};
};
}

#endif /* end of include guard: PILOT_BASE_H_SYAGZKV9 */
