#ifndef PILOT_BASE_H_SYAGZKV9
#define PILOT_BASE_H_SYAGZKV9

#include "Ship.h"

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
}

#endif /* end of include guard: PILOT_BASE_H_SYAGZKV9 */
