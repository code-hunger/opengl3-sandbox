#ifndef VERTEX_ARRAY
#define VERTEX_ARRAY

#include "GL/glew.h" // GL-types
#include <cstdio>
#include <vector>
using std::vector;

class VertexArray
{
	const vector<float> points;
	const vector<unsigned> indices;
	GLuint VAO = 0, VBO = 0, EBO = 0;
	void initBuffer(GLenum type, GLulong size, const GLvoid *data,
	                GLuint *buffer) const;

	void enableVertexArray(GLuint loc, GLint size, GLenum type, GLuint stride,
	                       GLuint start) const;

public:
	VertexArray(const float *points, int point_count,
	            const unsigned *indices = NULL, int indices_count = 0);

	void build(GLushort dimention, bool hasColor = true);

	void draw(GLenum mode, GLulong start = 0, GLsizei count = 0) const;

	GLuint getVAO() const { return VAO; }
	virtual ~VertexArray();
};

#endif /* end of include guard: VERTEX_ARRAY */
