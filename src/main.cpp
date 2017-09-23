#include "graphics/GlfwWrapper.h"
#include "graphics/Window.h"

#include "logger/logger.h"

#include "ConfigFactory/ConfigFactory.h"

bool GlfwWrapper::exists = false;

template <> void ConfigFactory::process(Window&) const;

int main(int argc, const char** argv)
{
	try {
		ConfigFactory factory{argc, argv};

		auto glfw = std::make_shared<GlfwWrapper>();
		Window window{glfw};
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
