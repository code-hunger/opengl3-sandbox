#ifndef SCREEN_H_4ZGYXQ9E
#define SCREEN_H_4ZGYXQ9E

#include "graphics/State.h"

struct Screen
{
	virtual void work(double deltaTime, State& state) = 0;
	virtual ~Screen(){};
};

#endif /* end of include guard: SCREEN_H_4ZGYXQ9E */
