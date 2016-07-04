#ifndef HOMESCREEN_H_A3CBSROG
#define HOMESCREEN_H_A3CBSROG

#include "Maze.h"
#include "Screen.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

class HomeScreen : public Screen
{
public:
	HomeScreen();
	virtual ~HomeScreen();

	void update(double deltaTime, State &state);
	void render(double deltaTime, const State &state);

private:
	ShaderProgram shaderProgram{};
	VertexArray va;
	Maze maze;
};

#endif /* end of include guard: HOMESCREEN_H_A3CBSROG */
