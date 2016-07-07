#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "VertexArray.h"
#include "geometry.h"

#include <unordered_set>

struct Hash {
	size_t operator()(const WeightLine2 &line) const
	{
		return static_cast<size_t>(line.a.x * line.a.y + line.calcSquaredLen());
	}
};

typedef std::unordered_set<WeightLine2, Hash> Lines;

class Maze
{
public:
	Maze(Lines paths);
	void draw(GLenum mode);
	virtual ~Maze();

private:
	Lines paths;
	VertexArray vertArray;
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
