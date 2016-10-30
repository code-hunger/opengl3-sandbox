#ifndef GAME_H
#define GAME_H

#include <memory>

#include "Screen.h"
#include "graphics/include/Renderer.h"

typedef unsigned short ushort;

class Game : public Renderer
{
public:
	Game(ushort maze_id, bool dont_join_lines, ushort max_lines);
	void render(double deltaTime, State& state) const;

	static Game fromCmd(int, char**);

	Game(const Game&) = delete;
	Game operator=(const Game&) = delete;

	Game(Game&&) = default;

private:
	std::unique_ptr<Screen> screen;
};

#endif /* end of include guard: GAME_H */
