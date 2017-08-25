#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <memory>

class Screen;
class GameplayScreen;
struct State;

typedef unsigned short ushort;

class ScreenManager
{
public:
	ScreenManager(GameplayScreen&&);

	void render(double deltaTime, State& state) const;

	ScreenManager(const ScreenManager&) = delete;
	ScreenManager operator=(const ScreenManager&) = delete;

	ScreenManager(ScreenManager&&);

	~ScreenManager();

private:
	// Do I need the GlfwWrapper to ensure opengl calls are valid?
	// std::shared_ptr<GlfwWrapper> glfwWrapper;

	std::unique_ptr<Screen> screen;
};

#endif /* end of include guard: SCREENMANAGER_H */
