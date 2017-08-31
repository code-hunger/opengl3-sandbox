#include "ConfigFactory.h"

#include "graphics/Window.h"
#include "ScreenManager.h"
#include "GameplayScreen.h"

#include "Maze.h"
#include "maze_builder/builder.h"
#include "math/types.h"

#include "Data.h"

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

template <> ScreenManager ConfigFactory::produce() const;

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
