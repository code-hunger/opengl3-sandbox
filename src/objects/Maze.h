#ifndef MAZE_H_UAWB7QD4
#define MAZE_H_UAWB7QD4

#include "VertexArray.h"
#include "geometry.h"

#include <unordered_set>

/* namespace std */
/* { */
/* template <> struct hash<WeightLine2> { */
/* 	size_t operator()(const WeightLine2 &line) const */
/* 	{ */
/* 		return static_cast<unsigned>(line.len_squared()); */
/* 	} */
/* }; */
/* } */

struct Hash {
	size_t operator()(const WeightLine2 &line) const
	{
		return static_cast<size_t>(line.a.x * line.a.y + line.len());
	}
};

typedef std::unordered_set<WeightLine2, Hash> Lines;

class Maze
{
public:
	Maze(Lines paths);
	void draw();
	virtual ~Maze();

private:
	Lines paths;
	VertexArray vertArray;
};

#endif /* end of include guard: MAZE_H_UAWB7QD4 */
