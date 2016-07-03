#ifndef GAME_H
#define GAME_H

#include "Renderer.h"
#include "ShaderProgram.h"
#include "State.h"
#include "VertexArray.h"
#include "Window.h"

class Game : public Renderer
{
public:
	Game();
	virtual ~Game();
	void render(double deltaTime, const State &state) const;

private:
	ShaderProgram shaderProgram{};
	VertexArray va;
};

#endif /* end of include guard: GAME_H */
