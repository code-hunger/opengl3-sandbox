#ifndef HOMESCREEN_H_A3CBSROG
#define HOMESCREEN_H_A3CBSROG

#include "Screen.h"
#include "graphics/include/ShaderProgram.h"

#include <memory>
#include <vector>

struct Drawable;

typedef unsigned short ushort;

class HomeScreen : public Screen
{
public:
	HomeScreen(ushort maze_id, bool join_it, ushort max_lines);
	virtual ~HomeScreen();

	void work(double deltaTime, State& state);

private:
	ShaderProgram shaderProgram{};
	std::vector<std::unique_ptr<Drawable>> screen_elements;
};

#endif /* end of include guard: HOMESCREEN_H_A3CBSROG */
