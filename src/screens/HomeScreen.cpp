#include "HomeScreen.h"
#include "Shader.h"
#include "config.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <cstdio>

inline VertexArray getVa()
{
    puts("Creating vertex array for the triangle...");
	GLfloat points[] = {0, 0, 1, 1, 0, 1};
	return {points, sizeof (points)/sizeof (GLfloat)};
}

inline Maze getMaze()
{
	Lines lines;
	WeightLine2 line;
	line = {{1, 0}, 1, {-3, 1}, 1};
	lines.insert(Lines::value_type(line));
	line = {{-1, 1}, 1, {-4, 2}, 1};
	lines.insert(Lines::value_type(line));
	return {lines};
}

HomeScreen::HomeScreen() : va(getVa()), maze(getMaze())
{
	printf("Creating shader programms...\n");

	Shader frag_sh(readFile(SHADER_DIRECTORY "/fragment_shader.glsl"),
	               GL_FRAGMENT_SHADER),
	    vert_sh(readFile(SHADER_DIRECTORY "/vertex_shader.glsl"),
	            GL_VERTEX_SHADER);

	frag_sh.compile();
	vert_sh.compile();

	shaderProgram.attachShader(vert_sh.id);
	shaderProgram.attachShader(frag_sh.id);
	shaderProgram.link();

	va.build(2, false);
}

void HomeScreen::update(const double deltaTime, State &state)
{
	if (state.keys[GLFW_KEY_ESCAPE]) {
		state.shouldClose = true;
	}
}

void HomeScreen::render(const double deltaTime, const State &state)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram.use();

	va.draw(GL_TRIANGLES);
	maze.draw();
}

HomeScreen::~HomeScreen() {}
