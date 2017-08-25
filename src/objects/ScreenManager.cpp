#include "ScreenManager.h"

#include "GameplayScreen.h"

#include "maze_builder/builder.h"

math::WideRoads fetchLinesFromMaze(unsigned maze_id);

Maze getMazeFromFile(ushort maze_id, bool join_it, ushort max_lines)
{
	auto lines = fetchLinesFromMaze(maze_id);

	const auto& walls = Builder{join_it, max_lines}.build_from_paths(lines);
	return Maze::build(std::move(lines), walls);
}

ScreenManager::ScreenManager(GameplayScreen&& screen)
    : screen(std::make_unique<GameplayScreen>(std::move(screen)))
{
}

ScreenManager::ScreenManager(ushort mazeId, bool noJoinLines, ushort maxLines)
    : ScreenManager(getMazeFromFile(mazeId, noJoinLines, maxLines))
{
}

ScreenManager::ScreenManager(ScreenManager&&) = default;

void ScreenManager::render(const double deltaTime, State& state) const
{
	screen->work(deltaTime, state);
}

ScreenManager::~ScreenManager() = default;
