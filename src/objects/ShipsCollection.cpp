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

	static float angle = 0;
	angle += time;
	if (angle > 3.141592653589f) angle -= 6.283185307179f;

	using namespace glm;

	auto model = rotate(angle, vec3(0.0f, 0.0f, 1.0f));
	model = translate(vec3(50.0f, 30.0f, 0.0f)) * model;

	shaderProgram.setUniformMatrix("model", glm::value_ptr(model));
}

void ShipsCollection::draw()
{
	shaderProgram.use();

	vertexArray.draw(GL_TRIANGLE_STRIP);
}
