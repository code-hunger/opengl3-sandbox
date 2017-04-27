#ifndef VERTEX_ARRAY
#define VERTEX_ARRAY

#include <vector>
using std::vector;

typedef unsigned long ulong;
typedef unsigned int uint;

class VertexArray
{
	const vector<float> points;
	const vector<unsigned> indices;
	uint VAO = 0, VBO = 0, EBO = 0;
	void initBuffer(uint type, ulong size, const void* data,
	                uint* buffer) const;

	void enableVertexArray(uint loc, int size, uint type, uint stride,
	                       uint start) const;

public:
	VertexArray(vector<float> points, vector<unsigned> indices = {},
	            bool is_3d = true, bool hasColor = true);

	void draw(uint mode, ulong start = 0, int count = 0) const;

	uint getVAO() const { return VAO; }
};

#endif /* end of include guard: VERTEX_ARRAY */
