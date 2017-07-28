#include "ShipsCollection.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

ShipsCollection::ShipsCollection()
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
	}
}

void ShipsCollection::draw(const Ship& ship)
{
	auto model = glm::rotate(glm::translate(glm::vec3(ship.x, ship.y, 0)),
	                         ship.getDirection(), glm::vec3(0.f, 0.f, 1.f));

	shaderProgram.setUniformMatrix("model", glm::value_ptr(model));
	vertexArray.draw(GL_TRIANGLE_STRIP);
}
