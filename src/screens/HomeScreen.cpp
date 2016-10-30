#include "HomeScreen.h"
#include "graphics/include/Shader.h"
#include "graphics/include/config.h"
#include "graphics/include/utils.h"
#include "math/include/geometry_io.h"
#include "maze_builder/include/builder.h"
#include <GLFW/glfw3.h>
#include <Maze.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Maze getMazeFromFile(ushort maze_id, bool join_it, ushort max_lines)
{
	std::string fileName(MAZE_DIRECTORY "/maze");
	fileName += std::to_string(static_cast<unsigned>(maze_id));
	fileName += ".txt";
	std::ifstream input(fileName);

	if (input.fail()) {
		throw "File does not exist!";
	}

	WideRoad2 line{{}, {}};
	Ways lines;
	while (input >> line) {
		lines.push_back(line);
	}
	printf("Fetched %lu lines in maze!\n", lines.size());

	return Maze::build(lines, {join_it, max_lines});
}

HomeScreen::HomeScreen(ushort maze_id, bool join_it,
                       ushort max_lines)
    : maze(getMazeFromFile(maze_id, join_it, max_lines))
{
	Shader frag_sh(readFile(SHADER_DIRECTORY "/fragment_shader.glsl").c_str(),
	               GL_FRAGMENT_SHADER),
	    vert_sh(readFile(SHADER_DIRECTORY "/vertex_shader.glsl").c_str(),
	            GL_VERTEX_SHADER);

	if (!frag_sh.tryToCompile()) {
		printf("Frag Shader compilation failed: %s", frag_sh.infoLog);
		throw;
	}

	if (!vert_sh.tryToCompile()) {
		printf("Vert Shader compilation failed: %s", vert_sh.infoLog);
		throw;
	}

	shaderProgram.attachShader(vert_sh.id);
	shaderProgram.attachShader(frag_sh.id);
	shaderProgram.link();

	glm::mat4 proj = glm::ortho(0.f, 100.f, 0.f, 100.f, 0.1f, -.1f);
	const GLfloat* const matrix = glm::value_ptr(proj);

	shaderProgram.use();
	shaderProgram.setUniformMatrix("model_view_projection", matrix);
}

void HomeScreen::update(const double deltaTime, State& state)
{
	(void)deltaTime;
	if (state.keys[GLFW_KEY_ESCAPE]) {
		state.shouldClose = true;
	}
}

void HomeScreen::render(const double deltaTime, const State& state)
{
	(void)deltaTime;
	(void)state;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	maze.draw(GL_LINES);
}

HomeScreen::~HomeScreen() {}
