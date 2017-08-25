#ifndef CONFIG_FACTORY_H_EQA17KCO
#define CONFIG_FACTORY_H_EQA17KCO

#include "ScreenManager.h"
#include "graphics/Window.h"

struct ConfigFactory
{
	ushort maze_id, max_lines;
	bool no_join_lines, print_fps;

public:
	ConfigFactory(int argc, char** argv);

	template <typename T> T produce() { return {}; };

	template <typename T> void process(T&) {}
};

template <> inline ScreenManager ConfigFactory::produce()
{
	return {maze_id, no_join_lines, max_lines};
}

template <> inline void ConfigFactory::process(Window& window)
{
	window.setPrintFps(print_fps);
}

#endif /* end of include guard: CONFIG_FACTORY_H_EQA17KCO */
