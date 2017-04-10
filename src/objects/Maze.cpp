#include "Maze.h"

#define MAX_LINES 60

using namespace math;

VertexArray wallsToVertArr(const ColorSegmentList& walls);

Maze Maze::build(WideRoads&& ways, const ColorSegmentList& walls)
{
	if (walls.size() > MAX_LINES) {
		throw "Maximum allowed line number exceeded";
	}

	return {ways, walls, wallsToVertArr(walls)};
}

VertexArray wallsToVertArr(const ColorSegmentList& walls)
{
	// 12 floats per line: per point 3x position and 3x color
	float points[MAX_LINES * 12];

	int i = 0;
	for (const auto& _wall : walls) { // Copy wallsP into points[]
		auto wall = &_wall;
		points[i++] = wall->line.a.x;
		points[i++] = wall->line.a.y;
		points[i++] = 0;
		points[i++] = wall->color.r;
		points[i++] = wall->color.g;
		points[i++] = wall->color.b;

		points[i++] = wall->line.b.x;
		points[i++] = wall->line.b.y;
		points[i++] = 0;
		points[i++] = wall->color.r;
		points[i++] = wall->color.g;
		points[i++] = wall->color.b;
	}

	return {vector<float>{points, points + i}};

	/*for (const auto& path : ways) {
	  points[i++] = path.a.point.x;
	  points[i++] = path.a.point.y;
	  points[i++] = 0;
	  points[i++] = 0.7f;
	  points[i++] = 0.7f;
	  points[i++] = 0.7f;
	  points[i++] = path.b.point.x;
	  points[i++] = path.b.point.y;
	  points[i++] = 0;
	  points[i++] = 0.7f;
	  points[i++] = 0.7f;
	  points[i++] = 0.7f;
	  }*/
}

Maze::Maze(WideRoads paths, ColorSegmentList walls, VertexArray vertArray)
    : paths(paths), walls(walls), vertArray(vertArray)
{
}

void Maze::draw(GLenum mode) { vertArray.draw(mode); }
