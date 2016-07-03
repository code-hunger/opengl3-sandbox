#include "HomeScreen.h"
#include "Shader.h"
#include "config.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <cstdio>

inline VertexArray getVa()
{
	GLfloat points[] = {0, 0, 1, 1, 0, 1};

	VertexArray va{points, sizeof points};
	va.build(2, false);

	return va;
}

HomeScreen::HomeScreen() : va(getVa())
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
}

HomeScreen::~HomeScreen() {}
