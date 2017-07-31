#ifndef PILOT_BASE_H_SYAGZKV9
#define PILOT_BASE_H_SYAGZKV9

#include <map>

struct State;
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

struct keyboard_controlled : pilot_base
{

	keyboard_controlled(int left, int right, int forward)
	    : LEFT_KEY(left), RIGHT_KEY(right), FORWARD_KEY(forward){};

	keyboard_controlled(const keyboard_controlled&) = delete;
	void operator=(const keyboard_controlled&) = delete;

	void operator()(Ship&) override;

	void setState(const State& state) { this->state = &state; }

private:
	const State* state = nullptr;
	const int LEFT_KEY, RIGHT_KEY, FORWARD_KEY;
};
}

#endif /* end of include guard: PILOT_BASE_H_SYAGZKV9 */
