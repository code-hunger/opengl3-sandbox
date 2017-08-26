#include "ScreenManager.h"

#include "graphics/GlfwWrapper.h"
#include "graphics/Window.h"

#include "logger/logger.h"

#include "ConfigFactory.h"

bool GlfwWrapper::exists = false;

int main(int argc, const char** argv)
{
	try {
		GlfwWrapper wrapper;
		auto window = wrapper.acquireWindow<Window>();

		ConfigFactory factory{argc, argv};
		factory.process(window);
	} catch (int e) {
		ERR << "Something bad happened. Code " << e;
		return e;
	} catch (const char* e) {
		ERR << "Something bad happened. Error text: " << e;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
