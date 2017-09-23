#include "ShipsCollection.h"

#include "Ship.h"

#include "math/Point.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <GLFW/glfw3.h>

ShipsCollection::ShipsCollection()
    : shaderProgram{"ship"}, ships{}, vertexArray{{
                                          -2, -1, 0, 1, 1, 1, // first point
                                          -2, 1,  0, 1, 1, 1, // second point
                                          2,  1,  0, 1, 1, 1, // third point
                                          2,  -1, 0, 1, 1, 1, // 4th point
                                          -2, -1, 0, 1, 1, 1, // 1st point again
                                      }}
{
	shaderProgram.link();

	glm::mat4 proj = glm::ortho(0.f, 100.f, 0.f, 100.f, 0.1f, -.1f);
	const GLfloat* const matrix = glm::value_ptr(proj);

	shaderProgram.setUniformMatrix("view_projection", matrix);
}

void ShipsCollection::update(const State& state, double time)
{
	for (Ship& ship : ships) {
		ship.update(state, static_cast<float>(time));
		ship.core.update(time);
	}
}

void ShipsCollection::draw(const Ship& ship)
{
	auto model =
	    glm::rotate(glm::translate(glm::vec3(ship.getX(), ship.getY(), 0)),
	                ship.getDirection(), glm::vec3(0.f, 0.f, 1.f));

	shaderProgram.setUniformMatrix("model", glm::value_ptr(model));
	vertexArray.draw(GL_TRIANGLE_STRIP);
}

ushort ShipsCollection::addShip(const math::Point2& location)
{
	ships.emplace_back(Ship{ShipCore{location}});
	return static_cast<ushort>(ships.size() - 1);
}

Ship& ShipsCollection::operator[](ushort index) { return ships[index]; }

void ShipsCollection::draw()
{
	shaderProgram.use();

	for (auto& ship : ships)
		draw(ship);
}

ShipsCollection::~ShipsCollection() = default;
