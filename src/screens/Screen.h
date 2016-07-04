#ifndef SCREEN_H_4ZGYXQ9E
#define SCREEN_H_4ZGYXQ9E

#include "State.h"

class Screen
{
public:
	Screen() {}
	virtual void update(double deltaTime, State &state) = 0;
	virtual void render(double deltaTime, const State &state) = 0;
	virtual ~Screen(){};
};

#endif /* end of include guard: SCREEN_H_4ZGYXQ9E */
