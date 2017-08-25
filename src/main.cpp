#include "ScreenManager.h"

#include "graphics/GlfwWrapper.h"
#include "graphics/Window.h"

#include "logger/logger.h"

#include "tclap/CmdLine.h"

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
		auto window = wrapper.acquireWindow<Window>();

		// a Window is needed to construct a ScreenManager
		ScreenManager screenManager = screenManagerFromCmd(argc, argv);

		window.run(screenManager, renderFunction);
	} catch (int e) {
		ERR << "Something bad happened. Code " << e;
		return e;
	} catch (const char* e) {
		ERR << "Something bad happened. Error text: " << e;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
