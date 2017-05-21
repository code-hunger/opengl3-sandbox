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
		ship.update(state, time);
	}
}

void ShipsCollection::draw()
{
	shaderProgram.use();

	for (auto& ship : ships) {
		auto model = glm::translate(glm::vec3(ship.getX(), ship.getY(), 0));
		shaderProgram.setUniformMatrix("model", glm::value_ptr(model));
		vertexArray.draw(GL_TRIANGLE_STRIP);
	}
}
