#include "HomeScreen.h"
#include "Shader.h"
#include "config.h"
#include "geometry_io.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <fstream>

inline VertexArray getVa()
{
	puts("Creating vertex array for the triangle...");
	GLfloat points[] = {0, 0, 1, 1, 0, 1};
	return {points, sizeof(points) / sizeof(GLfloat)};
}

Maze getMazeFromFile(const char *fileName = MAZE_DIRECTORY "/maze1.txt")
{
	puts("Get maze from file!");
	std::ifstream input(fileName);
	WeightLine2 line;
	Lines lines;
	while (input >> line) {
		puts("Fetching maze line...");
		lines.insert(Lines::value_type(line));
	}
	return {lines};
}

HomeScreen::HomeScreen() : va(getVa()), maze(getMazeFromFile())
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
