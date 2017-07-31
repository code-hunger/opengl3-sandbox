#ifndef HOMESCREEN_H_A3CBSROG
#define HOMESCREEN_H_A3CBSROG

#include "Maze.h"
#include "Screen.h"
#include "ShipsCollection.h"
#include "graphics/ShaderProgram.h"

typedef unsigned short ushort;

class GameplayScreen : public Screen
{
public:
	GameplayScreen(ushort maze_id, bool join_it, ushort max_lines);
	virtual ~GameplayScreen();

	void work(double deltaTime, State& state);

private:
	Maze maze;
	ShipsCollection ships{};

	ushort mainPlayer;

	std::shared_ptr<pilot_base> pilot;
};

#endif /* end of include guard: HOMESCREEN_H_A3CBSROG */
