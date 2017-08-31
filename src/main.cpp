#include "graphics/GlfwWrapper.h"
#include "graphics/Window.h"

#include "logger/logger.h"

#include "ConfigFactory/ConfigFactory.h"

bool GlfwWrapper::exists = false;

template <> void ConfigFactory::process(Window& window) const;

int main(int argc, const char** argv)
{
	try {
		ConfigFactory factory{argc, argv};

		auto wrapper = factory.produce<GlfwWrapper>();
		auto window = wrapper.acquireWindow<Window>();
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
