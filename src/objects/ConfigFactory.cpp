#include "ConfigFactory.h"

#include "graphics/Window.h"
#include "ScreenManager.h"
#include "GameplayScreen.h"

#include "Maze.h"
#include "maze_builder/builder.h"
#include "math/types.h"

#include "tclap/CmdLine.h"

struct ConfigFactory::Data
{
	ushort maze_id, max_lines;
	bool no_join_lines, print_fps;

	static Data fromCli(int argc, const char** argv)
	{
		using namespace TCLAP;

		CmdLine cmd("ScreenManager title", ' ', "0.1");
		UnlabeledValueArg<ushort> maze_id("maze_id", "Maze id to load", false,
		                                  1, "number", cmd);
		ValueArg<ushort> max_lines("m", "max_lines", "Max ways to be added",
		                           false, 0, "count", cmd);
		SwitchArg no_join_lines("n", "nojoin", "Don't join intersecting lines",
		                        cmd, true),
		    print_fps("f", "print-fps", "prints fps", cmd);

		cmd.parse(argc, argv);

		return {maze_id.getValue(), max_lines.getValue(),
		        no_join_lines.getValue(), print_fps.getValue()};
	}
};

ConfigFactory::ConfigFactory(int argc, const char** argv)
    : data(new Data(Data::fromCli(argc, argv)))
{
}

math::WideRoads fetchLinesFromMaze(unsigned maze_id);

template <> Builder ConfigFactory::produce() const
{
	return {!data->no_join_lines, data->max_lines};
}

template <> Maze ConfigFactory::produce() const
{
	auto lines = fetchLinesFromMaze(data->maze_id);

	return {lines, produce<Builder>().build_from_paths(lines)};
}

template <> ScreenManager ConfigFactory::produce() const
{
	return {produce<Maze>()};
}

void renderFunction(void* renderObject, double deltaTime, State& state)
{
	static_cast<ScreenManager*>(renderObject)->render(deltaTime, state);
}

template <> void ConfigFactory::process(Window& window) const
{
	window.setPrintFps(data->print_fps);

	window.run(produce<ScreenManager>(), renderFunction);
}

ConfigFactory::~ConfigFactory() { delete data; }
