#ifndef SHIPSCOLLECTION_H_YWCRVLBE
#define SHIPSCOLLECTION_H_YWCRVLBE

#include "Ship.h"
#include "graphics/include/State.h"
#include <vector>

class ShipsCollection
{
public:
	ShipsCollection(){};

	void addShip(Ship&& ship) { ships.emplace_back(std::move(ship)); };

	void update(const State&, double) {}
	void draw() const
	{
		shaderProgram.use();
		vertexArray.draw(GL_TRIANGLE_STRIP);
	}

private:
	ShaderProgram shaderProgram{};
	std::vector<Ship> ships{};

	VertexArray vertexArray{{
	    1, 1, 0, 1,  1, 1, 1, 11, 0, 1, 1, 1, 11, 11, 0,
	    1, 1, 1, 11, 1, 0, 1, 1,  1, 1, 1, 0, 1,  1,  1,
	}};
};

#endif /* end of include guard: SHIPSCOLLECTION_H_YWCRVLBE */
