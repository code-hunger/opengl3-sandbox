#include "ScreenManager.h"

#include "GameplayScreen.h"

ScreenManager::ScreenManager(GameplayScreen&& screen)
    : screen(std::make_unique<GameplayScreen>(std::move(screen)))
{
}

ScreenManager::ScreenManager(ScreenManager&&) = default;

void ScreenManager::render(const double deltaTime, State& state) const
{
	screen->work(deltaTime, state);
}

ScreenManager::~ScreenManager() = default;
