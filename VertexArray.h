#ifndef VERTEX_ARRAY
#define VERTEX_ARRAY

#include <vector>
using std::vector;

class VertexArray
{
public:
	VertexArray(float *points, int point_count, unsigned int *indices,
	            int index_count)
	    : points(points, points + point_count +1),
	      indices(indices, indices + index_count + 1)
	{
		printf("Vertex array ctor initialized successfully!\n");
	};
	void build();
	void draw(GLenum mode, long unsigned int start, GLsizei count);
	GLuint getVAO() { return VAO; }
	virtual ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    void operator=(const VertexArray&) = delete;

private:
	vector<float> points;
	vector<unsigned int> indices;
	GLuint VAO = 0, VBO = 0, EBO = 0;
	void initBuffer(GLenum type, long unsigned int size, void *data,
	                GLuint *buffer);

	void enableVertexArray(GLuint location, GLint size, GLenum type,
	                       unsigned int stride, unsigned int start);
	void enableVertexArray(GLuint location, GLint size, unsigned int stride,
	                       unsigned int start)
	{
		enableVertexArray(location, size, GL_FLOAT, stride, start);
	}
};

#endif /* end of include guard: VERTEX_ARRAY */
