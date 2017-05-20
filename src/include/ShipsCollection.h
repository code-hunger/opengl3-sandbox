#ifndef SHIPSCOLLECTION_H_YWCRVLBE
#define SHIPSCOLLECTION_H_YWCRVLBE

#include "Ship.h"
#include "graphics/include/State.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ShipsCollection
{
public:
	ShipsCollection() { shaderProgram.link(); }

	void addShip(Ship&& ship)
	{
		glm::mat4 proj = glm::ortho(0.f, 100.f, 0.f, 100.f, 0.1f, -.1f);
		const GLfloat* const matrix = glm::value_ptr(proj);

		shaderProgram.setUniformMatrix("model_view_projection", matrix);
		ships.emplace_back(std::move(ship));
	}

	void update(const State&, double) {}

	void draw() const
	{
		shaderProgram.use();
		vertexArray.draw(GL_TRIANGLE_STRIP);
	}

private:
	ShaderProgram shaderProgram{"ship"};
	std::vector<Ship> ships{};

	VertexArray vertexArray{{
	    0, 0, 0, 1,  1, 1, 0, 10, 0, 1, 1, 1, 10, 10, 0,
	    1, 1, 1, 10, 0, 0, 1, 1,  1, 0, 0, 0, 1,  1,  1,
	}};
};

#endif /* end of include guard: SHIPSCOLLECTION_H_YWCRVLBE */
