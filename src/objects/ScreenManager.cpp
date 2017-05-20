#include "ScreenManager.h"

#include "GameplayScreen.h"

ScreenManager::ScreenManager(ushort mazeId, bool noJoinLines, ushort maxLines)
    : screen(std::make_unique<GameplayScreen>(mazeId, !noJoinLines, maxLines))
{
}

ScreenManager::ScreenManager(ScreenManager&&) = default;

void ScreenManager::render(const double deltaTime, State& state) const
{
	screen->work(deltaTime, state);
}

ScreenManager::~ScreenManager() = default;
