#ifndef HOMESCREEN_H_A3CBSROG
#define HOMESCREEN_H_A3CBSROG

#include "Maze.h"
#include "Screen.h"
#include "graphics/include/ShaderProgram.h"

typedef unsigned short ushort;

class HomeScreen : public Screen
{
public:
	HomeScreen(ushort maze_id, bool join_it, ushort max_lines);
	virtual ~HomeScreen();

	void work(double deltaTime, State& state);

private:
	Maze maze;
};

#endif /* end of include guard: HOMESCREEN_H_A3CBSROG */
