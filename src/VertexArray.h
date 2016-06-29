#ifndef VERTEX_ARRAY
#define VERTEX_ARRAY

#include <vector>
using std::vector;

class VertexArray
{
public:
	VertexArray(float *points, int p_count, unsigned *indices, int i_count)
	    : points(points, points + p_count),
	      indices(indices, indices + i_count)
	{
		printf("Vertex array ctor initialized successfully!\n");
	};

	VertexArray(float *points, int point_count)
	    : points(points, points + point_count), indices()
	{
		printf("Vertex array ctor initialized successfully!\n");
	};

	void build(GLushort dimention, bool hasColor = true);

	void draw(GLenum mode, GLulong start, GLsizei count) const;
	void draw(GLenum mode, GLulong start) const
	{
		if (indices.size())
			draw(mode, start, static_cast<signed>(indices.size()));
		else
			draw(mode, start, static_cast<signed>(points.size()));
	}

	GLuint getVAO() { return VAO; }
	virtual ~VertexArray();

private:
	vector<float> points;
	vector<unsigned> indices;
	GLuint VAO = 0, VBO = 0, EBO = 0;
	void initBuffer(GLenum type, GLulong size, GLvoid *data, GLuint *buffer);

	void enableVertexArray(GLuint loc, GLint size, GLenum type, GLuint stride,
	                       GLuint start);
	void enableVertexArray(GLuint loc, GLint size, GLuint stride, GLuint start)
	{
		enableVertexArray(loc, size, GL_FLOAT, stride, start);
	}
};

#endif /* end of include guard: VERTEX_ARRAY */
