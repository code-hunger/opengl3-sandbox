#include <cstdio>

#include "VertexArray.h"

void VertexArray::initBuffer(GLenum type, GLulong size, void *data,
                             GLuint *buffer)
{
	glGenBuffers(1, buffer);
	glBindBuffer(type, *buffer);
	glBufferData(type, static_cast<signed>(size), data, GL_STATIC_DRAW);
}

void VertexArray::enableVertexArray(GLuint location, GLint size, GLenum type,
                                    GLuint stride, GLuint start)
{
	glVertexAttribPointer(location, size, type, GL_FALSE,
	                      static_cast<GLsizei>(stride * sizeof type),
	                      reinterpret_cast<GLvoid *>(sizeof(type) * start));
	glEnableVertexAttribArray(location);
}

void VertexArray::build(GLushort dimention, bool hasColor)
{
	printf("Building vertex array\n");
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	initBuffer(GL_ARRAY_BUFFER, points.size(), points.data(), &VBO);
	if (indices.size() > 0) {
		initBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(),
		           &EBO);
	}

	unsigned stride = dimention;
	if (hasColor) stride += 3; // color has 3 components

	enableVertexArray(0, dimention, stride, 0);

	if (hasColor) {
		enableVertexArray(1, dimention, stride, dimention);
	}

	glBindVertexArray(0); // unbind
	printf("Vertex array built!\n");
}

void VertexArray::draw(GLenum mode, GLulong start, GLsizei count) const
{
	glBindVertexArray(VAO);
	if (indices.size()) {
		glDrawElements(mode, count, GL_UNSIGNED_INT,
		               reinterpret_cast<GLvoid *>(start));
	} else {
		glDrawArrays(mode, static_cast<signed>(start), count);
	}
	glBindVertexArray(0);
}

VertexArray::~VertexArray() {}
