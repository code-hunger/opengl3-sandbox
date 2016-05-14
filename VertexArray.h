#ifndef VERTEX_ARRAY
#define VERTEX_ARRAY

#include <vector>
using std::vector;

class VertexArray
{
public:
	VertexArray(float *points, int point_count, unsigned int *indices,
	            int index_count)
	    : points(points, points + point_count),
	      indices(indices, indices + index_count)
	{
		printf("Vertex array ctor initialized successfully!\n");
	};
	void build(unsigned dimention);
	void draw(GLenum mode, long unsigned start, GLsizei count);
	void draw(GLenum mode, long unsigned start) { draw(mode, start, static_cast<int>(indices.size())); }

	GLuint getVAO() { return VAO; }
	virtual ~VertexArray();

	VertexArray(const VertexArray &) = delete;
	void operator=(const VertexArray &) = delete;

private:
	vector<float> points;
	vector<unsigned int> indices;
	GLuint VAO = 0, VBO = 0, EBO = 0;
	void initBuffer(GLenum type, long unsigned int size, void *data,
	                GLuint *buffer);

	void enableVertexArray(GLuint location, GLint size, GLenum type,
	                       unsigned stride, unsigned start);
	void enableVertexArray(GLuint location, GLint size, unsigned stride,
	                       unsigned start)
	{
		enableVertexArray(location, size, GL_FLOAT, stride, start);
	}
};

#endif /* end of include guard: VERTEX_ARRAY */
