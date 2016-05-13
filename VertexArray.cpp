#include <cstdio>
#include <GL/glew.h>

#include "VertexArray.h"

void VertexArray::initBuffer(GLenum type, long unsigned size, void *data,
                             GLuint *buffer)
{
	glGenBuffers(1, buffer);
	glBindBuffer(type, *buffer);
	glBufferData(type, static_cast<signed>(size), data, GL_STATIC_DRAW);
}

void VertexArray::enableVertexArray(GLuint location, GLint size, GLenum type,
                                    unsigned int stride, unsigned int start)
{
	glVertexAttribPointer(location, size, type, GL_FALSE,
	                      static_cast<GLsizei>(stride * sizeof type),
	                      reinterpret_cast<GLvoid *>(sizeof(type) * start));
	glEnableVertexAttribArray(location);
}

void VertexArray::build(unsigned dimention)
{
    printf("Building vertex array\n");
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	initBuffer(GL_ARRAY_BUFFER, points.size(), points.data(), &VBO);
	initBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), &EBO);

    unsigned stride = static_cast<unsigned>(dimention) + 3;
	enableVertexArray(0, static_cast<int>(dimention), stride, 0);
    enableVertexArray(1, static_cast<int>(dimention), stride, dimention);

	glBindVertexArray(0); // unbind
    printf("Vertex array built!\n");
}

void VertexArray::draw(GLenum mode, long unsigned int start, GLsizei count)
{
	glBindVertexArray(VAO);
	glDrawElements(mode, count, GL_UNSIGNED_INT, reinterpret_cast<GLvoid *>(start));
	glBindVertexArray(0);
}

VertexArray::~VertexArray() {}
