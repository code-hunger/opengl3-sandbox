#include "ScreenManager.h"
#include "graphics/include/GlfwWrapper.h"
#include "graphics/include/Window.h"
#include "logger/include/logger.h"
#include "tclap/CmdLine.h"
#include <GL/glew.h>

#include <cstdio>

#include <iostream>

bool GlfwWrapper::exists = false;

ScreenManager screenManagerFromCmd(int argc, char** argv)
{
	using namespace TCLAP;

	CmdLine cmd("ScreenManager title", ' ', "0.1");
	UnlabeledValueArg<ushort> maze_id("maze_id", "Maze id to load", false, 1,
	                                  "number", cmd);
	ValueArg<ushort> max_lines("m", "max_lines", "Max ways to be added", false,
	                           0, "count", cmd);
	SwitchArg no_join_lines("n", "nojoin", "Don't join intersecting lines", cmd,
	                        true);
	cmd.parse(argc, argv);

	return {maze_id.getValue(), no_join_lines.getValue(), max_lines.getValue()};
}

void renderFunction(void* renderObject, double deltaTime, State& state)
{
	static_cast<ScreenManager*>(renderObject)->render(deltaTime, state);
}

int main(int argc, char** argv)
{
	try {
		GlfwWrapper wrapper;
		Window window = wrapper.acquireWindow();

		// a Window is needed to construct a ScreenManager
		ScreenManager screenManager = screenManagerFromCmd(argc, argv);

		window.run<ScreenManager>(screenManager, renderFunction);
	} catch (int e) {
		LOG("Something bad happened. Code %d\n", e);
		return e;
	} catch (const char* e) {
		LOG("Something bad happened. Error text: %s\n", e);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
