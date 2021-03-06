#include <GL/glew.h>

#include "VertexArray.h"

VertexArray::VertexArray(vector<float> points, vector<unsigned> indices,
                         bool is_3d, bool hasColor)
    : points(points), indices(indices)
{
	if (!is_3d || !hasColor) {
		throw "Only colorful 3d VertexArray supported!";
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	initBuffer(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]),
	           points.data(), &VBO);
	if (indices.size() > 0) {
		initBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]),
		           indices.data(), &EBO);
	}

	unsigned stride = is_3d ? 3 : 2;
	if (hasColor) stride += 3; // color has 3 components

	enableVertexArray(0, is_3d ? 3 : 2, GL_FLOAT, stride, 0);

	if (hasColor) {
		enableVertexArray(1, is_3d ? 3 : 2, GL_FLOAT, stride, is_3d ? 3 : 2);
	}

	glBindVertexArray(VAO);
	glBindVertexArray(0); // unbind
}

void VertexArray::initBuffer(GLenum type, GLulong size, const void* data,
                             GLuint* buffer) const
{
	glGenBuffers(1, buffer);
	glBindBuffer(type, *buffer);
	glBufferData(type, static_cast<signed>(size), data, GL_STATIC_DRAW);
}

void VertexArray::enableVertexArray(GLuint location, GLint size, GLenum type,
                                    GLuint stride, GLuint start) const
{
	glVertexAttribPointer(location, size, type, GL_FALSE,
	                      static_cast<GLsizei>(stride * sizeof type),
	                      reinterpret_cast<GLvoid*>(sizeof(type) * start));
	glEnableVertexAttribArray(location);
}

void VertexArray::draw(GLenum mode, GLulong start, GLsizei count) const
{
	glBindVertexArray(VAO);
	if (indices.size()) {
		if (count == 0) count = static_cast<signed>(indices.size());

		glDrawElements(mode, count, GL_UNSIGNED_INT,
		               reinterpret_cast<GLvoid*>(start));
	} else {
		if (count == 0) count = static_cast<signed>(points.size());

		glDrawArrays(mode, static_cast<signed>(start), count);
	}
	glBindVertexArray(0);
}

// VertexArray::~VertexArray() {}
