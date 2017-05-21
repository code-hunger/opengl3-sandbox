#ifndef SHIPSCOLLECTION_H_YWCRVLBE
#define SHIPSCOLLECTION_H_YWCRVLBE

#include "Ship.h"
#include "graphics/include/State.h"

#include <vector>

class ShipsCollection
{
public:
	ShipsCollection();

	void addShip(Ship&& ship) { ships.emplace_back(std::forward<Ship>(ship)); }

	void update(const State&, double);

	void draw();

private:
	ShaderProgram shaderProgram{"ship"};
	std::vector<Ship> ships{};

	VertexArray vertexArray{{
	    0, 0, 0, 1,  1, 1, 0, 10, 0, 1, 1, 1, 10, 10, 0,
	    1, 1, 1, 10, 0, 0, 1, 1,  1, 0, 0, 0, 1,  1,  1,
	}};
};

#endif /* end of include guard: SHIPSCOLLECTION_H_YWCRVLBE */
