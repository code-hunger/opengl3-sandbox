#include "ConfigFactory.h"
#include "tclap/CmdLine.h"

struct ConfigFactory::Data
{
	ushort maze_id, max_lines;
	bool no_join_lines, print_fps;
};

ConfigFactory::ConfigFactory(int argc, char** argv) : data(new Data)
{
	using namespace TCLAP;

	CmdLine cmd("ScreenManager title", ' ', "0.1");
	UnlabeledValueArg<ushort> maze_id("maze_id", "Maze id to load", false, 1,
	                                  "number", cmd);
	ValueArg<ushort> max_lines("m", "max_lines", "Max ways to be added", false,
	                           0, "count", cmd);
	SwitchArg no_join_lines("n", "nojoin", "Don't join intersecting lines", cmd,
	                        true),
	    print_fps("f", "print-fps", "prints fps", cmd);

	cmd.parse(argc, argv);

	data->maze_id = maze_id.getValue();
	data->no_join_lines = no_join_lines.getValue();
	data->max_lines = max_lines.getValue();
	data->print_fps = print_fps.getValue();
}

template <> ScreenManager ConfigFactory::produce()
{
	return {data->maze_id, data->no_join_lines, data->max_lines};
}

template <> void ConfigFactory::process(Window& window)
{
	window.setPrintFps(data->print_fps);
}

ConfigFactory::~ConfigFactory() { delete data; }
