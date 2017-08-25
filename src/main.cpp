#include "ScreenManager.h"

#include "graphics/GlfwWrapper.h"
#include "graphics/Window.h"

#include "logger/logger.h"

#include "ConfigFactory.h"

#include "tclap/CmdLine.h"

bool GlfwWrapper::exists = false;

void renderFunction(void* renderObject, double deltaTime, State& state)
{
	static_cast<ScreenManager*>(renderObject)->render(deltaTime, state);
}

int main(int argc, char** argv)
{
	try {
		GlfwWrapper wrapper;
		auto window = wrapper.acquireWindow<Window>();

		ConfigFactory factory{argc, argv};
		factory.process(window);

		// a Window is needed to construct a ScreenManager
		auto screenManager = factory.produce<ScreenManager>();

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
