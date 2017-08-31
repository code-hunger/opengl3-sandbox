#include "GameplayScreen.h"

#include "ConfigFactory/ConfigFactory.h"

#include "../src/objects/pilots/pilot_base.h"
#include "Maze.h"

#include "graphics/State.h"
#include "graphics/utils.h"
#include "logger/logger.h"

#include <GLFW/glfw3.h>

GameplayScreen::GameplayScreen(Maze&& maze)
    : maze(std::move(maze)), mainPlayer(ships.addShip(math::Point2{17, 37})),
      pilot(std::make_shared<pilots::line_follower>(ships[mainPlayer]))
{
	ships[mainPlayer].setPilot(std::make_shared<pilots::keyboard_controlled>(
	    GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_SPACE));
}

void update(const double deltaTime, State& state, ShipsCollection& ships,
            Ship& mainPlayer, std::shared_ptr<pilot_base> pilot)
{
	if (state.keys[GLFW_KEY_ESCAPE]) {
		state.shouldClose = true;
	}

	if (state.keys[GLFW_KEY_KP_ADD]) {
		ships.addShip({45, 50});
		ships->back().setPilot(pilot);
		static_cast<pilots::line_follower*>(&*pilot)->attach_ship(
		    ships->back());
	}

	static_cast<pilots::keyboard_controlled*>(&*mainPlayer.getPilot())
	    ->setState(state);
	ships.update(state, deltaTime);
}

void render(double, Maze& maze, ShipsCollection& ships)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	maze.draw(GL_LINES);
	ships.draw();
}

void GameplayScreen::work(const double deltaTime, State& state)
{
	update(deltaTime, state, ships, ships[mainPlayer], pilot);
	render(deltaTime, maze, ships);
}

template <> Maze ConfigFactory::produce() const;
template <> GameplayScreen ConfigFactory::produce() const {
	return produce<Maze>();
}

GameplayScreen::~GameplayScreen() {}
