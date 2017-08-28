#ifndef SHIPSCOLLECTION_H_YWCRVLBE
#define SHIPSCOLLECTION_H_YWCRVLBE

#include "Ship.h"

#include "graphics/ShaderProgram.h"
#include "graphics/VertexArray.h"

#include <deque>

struct State;

class ShipsCollection
{
public:
	ShipsCollection();

	/**
	 * Warning! Invalidates ship pointers.
	 */
	ushort addShip(math::Point2 location)
	{
		ships.emplace_back(Ship{location});
		return static_cast<ushort>(ships.size() - 1);
	}

	Ship& operator[](ushort index) { return ships[index]; }
	const auto& operator*() { return ships; }
	auto* operator-> () { return &ships; }

	void update(const State&, double);

	void draw()
	{
		shaderProgram.use();

		for (auto& ship : ships)
			draw(ship);
	}

private:
	ShaderProgram shaderProgram{"ship"};
	std::deque<Ship> ships{}; // Deque, in order to keep pointers to Ships valid
	                          // after push_back-ing. Consider storing indices
	                          // instead of pointers to resolve this problem and
	                          // get back to using vector.

	VertexArray vertexArray{{
	    -2, -2, 0, 1, 1, 1, // first point
	    -2, 2,  0, 1, 1, 1, // second point
	    2,  2,  0, 1, 1, 1, // third point
	    2,  -2, 0, 1, 1, 1, // 4th point
	    -2, -2, 0, 1, 1, 1, // 1st point again
	}};

	void draw(const Ship& ship);
};

#endif /* end of include guard: SHIPSCOLLECTION_H_YWCRVLBE */
