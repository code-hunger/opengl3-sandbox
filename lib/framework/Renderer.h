#ifndef APP_H_TMV30BLE
#define APP_H_TMV30BLE

#include "State.h"

struct Renderer
{
	virtual void render(double deltaTime, State&) const = 0;
	virtual ~Renderer(){};
};

#endif /* end of include guard: APP_H_TMV30BLE */
