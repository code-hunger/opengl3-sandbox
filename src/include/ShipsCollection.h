#ifndef SHIPSCOLLECTION_H_YWCRVLBE
#define SHIPSCOLLECTION_H_YWCRVLBE

#include "graphics/ShaderProgram.h"
#include "graphics/VertexArray.h"

#include "Ship.h"
#include "graphics/State.h"

#include <vector>

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

	void update(const State&, double);

	void draw()
	{
		shaderProgram.use();

		for (auto& ship : ships)
			draw(ship);
	}

private:
	ShaderProgram shaderProgram{"ship"};
	std::vector<Ship> ships{};

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
