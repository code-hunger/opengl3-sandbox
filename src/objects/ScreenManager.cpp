#include "ScreenManager.h"

#include "ConfigFactory/ConfigFactory.h"
#include "GameplayScreen.h"

#include "Ship.h"

#include "math/types.h"

ScreenManager::ScreenManager(GameplayScreen&& screen)
    : screen(std::make_unique<GameplayScreen>(std::move(screen)))
{
}

ScreenManager::ScreenManager(ScreenManager&&) = default;

void ScreenManager::render(const double deltaTime, State& state) const
{
	screen->work(deltaTime, state);
}

template <> GameplayScreen ConfigFactory::produce() const;
template <> ScreenManager ConfigFactory::produce() const
{
	return produce<GameplayScreen>();
}

ScreenManager::~ScreenManager() = default;
