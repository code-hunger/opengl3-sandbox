#include "Maze.h"
#include "logger/include/logger.h"

#include "graphics/include/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/include/config.h"
#include "graphics/include/utils.h"

#define MAX_LINES 100

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
	for (const ColorSegment2& wall : walls) { // Copy wallsP into points[]
		points[i++] = wall.line.a.x;
		points[i++] = wall.line.a.y;
		points[i++] = 0;
		points[i++] = wall.color.r;
		points[i++] = wall.color.g;
		points[i++] = wall.color.b;

		points[i++] = wall.line.b.x;
		points[i++] = wall.line.b.y;
		points[i++] = 0;
		points[i++] = wall.color.r;
		points[i++] = wall.color.g;
		points[i++] = wall.color.b;
	}

	return {vector<float>{points, points + i}};
}

Maze::Maze(const WideRoads& paths, const ColorSegmentList& walls,
           VertexArray&& vertArray)
    : paths(paths), walls(walls), vertArray(std::move(vertArray))
{
	shaderProgram.createShader("maze.frag", GL_FRAGMENT_SHADER);
	shaderProgram.createShader("maze.vert", GL_VERTEX_SHADER);

	shaderProgram.link();

	glm::mat4 proj = glm::ortho(0.f, 100.f, 0.f, 100.f, 0.1f, -.1f);
	const GLfloat* const matrix = glm::value_ptr(proj);

	shaderProgram.setUniformMatrix("model_view_projection", matrix);
}

void Maze::draw(uint mode)
{
	shaderProgram.use();
	vertArray.draw(mode);
}
