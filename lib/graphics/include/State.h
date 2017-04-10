#ifndef STATE_H_CODY3JAM
#define STATE_H_CODY3JAM

#include <memory>
#include <vector>

struct State
{
	int mouseX, mouseY;
	bool shouldClose;
	bool* const keys;
};

#endif /* end of include guard: STATE_H_CODY3JAM */
