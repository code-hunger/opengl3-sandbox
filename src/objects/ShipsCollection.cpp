#include "ShipsCollection.h"

ShipsCollection::ShipsCollection()
{
	shaderProgram.link();

	glm::mat4 proj = glm::ortho(0.f, 100.f, 0.f, 100.f, 0.1f, -.1f);
	const GLfloat* const matrix = glm::value_ptr(proj);

	shaderProgram.setUniformMatrix("model_view_projection", matrix);
}

void ShipsCollection::update(const State&, double) {}
