#include "GameplayScreen.h"

#include "graphics/utils.h"
#include "logger/logger.h"
#include "maze_builder/builder.h"

#include "../src/objects/pilots/pilot_base.h"
#include "Maze.h"

#include <GLFW/glfw3.h>

math::WideRoads fetchLinesFromMaze(unsigned maze_id);

Maze getMazeFromFile(ushort maze_id, bool join_it, ushort max_lines)
{
	auto lines = fetchLinesFromMaze(maze_id);

	const auto& walls = Builder{join_it, max_lines}.build_from_paths(lines);
	return Maze::build(std::move(lines), walls);
}

GameplayScreen::GameplayScreen(ushort maze_id, bool join_it, ushort max_lines)
    : maze(getMazeFromFile(maze_id, join_it, max_lines)),
      mainPlayer(ships.addShip(math::Point2{17, 37}))
{
}

void update(const double deltaTime, State& state, ShipsCollection& ships,
            Ship& mainPlayer)
{
	if (state.keys[GLFW_KEY_ESCAPE]) {
		state.shouldClose = true;
	}

	if (state.keys[GLFW_KEY_SPACE]) {
		mainPlayer.startMoving();
	} else {
		mainPlayer.stopMoving();
	}
	if (state.keys[GLFW_KEY_LEFT]) {
		mainPlayer.rotate(Rotation::LEFT);
	} else if (state.keys[GLFW_KEY_RIGHT]) {
		mainPlayer.rotate(Rotation::RIGHT);
	} else {
		mainPlayer.rotate(Rotation::NONE);
	}

	if (state.keys[GLFW_KEY_KP_ADD]) {
		ships.addShip({45, 50});
		ships->back().setPilot(
		    std::make_unique<pilots::follower>(mainPlayer));
		    // std::make_unique<pilots::follower>(ships[ships->size() - 2]));
		LOG << "New ship added. " << ships->size() << " ships now.";
	}

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
	update(deltaTime, state, ships, ships[mainPlayer]);
	render(deltaTime, maze, ships);
}

GameplayScreen::~GameplayScreen() {}
