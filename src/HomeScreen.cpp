#include "HomeScreen.h"
#include "Shader.h"
#include "config.h"
#include "utils.h"
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

void HomeScreen::update(double deltaTime, const State &state) {}

void HomeScreen::render(double deltaTime, const State &state)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	(void)deltaTime;
	(void)state;

	shaderProgram.use();

	va.draw(GL_TRIANGLES, 0);
	/* programs[0].use(); */

	/* srand(42); */

	/* vertArrays[2].draw(GL_TRIANGLES, 0); */

	/* vertArrays[2].draw(GL_TRIANGLES, 0); */
}

HomeScreen::~HomeScreen() {}
