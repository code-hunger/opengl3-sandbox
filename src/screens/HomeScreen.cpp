#include "HomeScreen.h"
#include "Shader.h"
#include "config.h"
#include "geometry_io.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Maze getMazeFromFile(unsigned short maze_id)
{
	puts("Get maze from file!");
	std::string fileName(MAZE_DIRECTORY "/maze");
	fileName += std::to_string(static_cast<unsigned>(maze_id));
	fileName += ".txt";
	puts(fileName.c_str());
	std::ifstream input(fileName);
	WideRoad2 line;
	Ways lines;
	while (input >> line) {
		puts("Fetching maze line...");
		lines.insert(Ways::value_type(line));
	}
	return Maze::fromPaths(lines);
}

HomeScreen::HomeScreen(unsigned short maze_id) : maze(getMazeFromFile(maze_id))
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

	glm::mat4 proj = glm::ortho(0.f, 100.f, 0.f, 66.f, 0.1f, -.1f);
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