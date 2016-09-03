#ifndef GAME_H
#define GAME_H

#include <memory>

#include "graphics/Renderer.h"
#include "Screen.h"
#include "graphics/ShaderProgram.h"
#include "graphics/State.h"
#include "graphics/VertexArray.h"
#include "graphics/Window.h"

class Game : public Renderer
{
public:
	Game(unsigned short maze_id);
	virtual ~Game();
	void render(double deltaTime, State& state) const;

	Game(const Game&) = delete;
	Game operator=(const Game&) = delete;

private:
	std::unique_ptr<Screen> screen;
};

#endif /* end of include guard: GAME_H */