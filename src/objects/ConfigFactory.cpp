#include "ConfigFactory.h"

#include "GameplayScreen.h"
#include "ScreenManager.h"
#include "graphics/Window.h"

#include "Maze.h"
#include "math/types.h"
#include "maze_builder/builder.h"

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

template <> Builder ConfigFactory::produce()
{
	return {!data->no_join_lines, data->max_lines};
}

template <> Maze ConfigFactory::produce()
{
	auto lines = fetchLinesFromMaze(data->maze_id);

	const auto& walls = produce<Builder>().build_from_paths(lines);
	return {std::move(lines), walls};
}

template <> ScreenManager ConfigFactory::produce() { return {produce<Maze>()}; }

void renderFunction(void* renderObject, double deltaTime, State& state)
{
	static_cast<ScreenManager*>(renderObject)->render(deltaTime, state);
}

template <> void ConfigFactory::process(Window& window)
{
	window.setPrintFps(data->print_fps);

	window.run(produce<ScreenManager>(), renderFunction);
}

ConfigFactory::~ConfigFactory() { delete data; }
