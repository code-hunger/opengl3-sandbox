#ifndef APP_H
#define APP_H

#include <vector>
#include "Window.h"
#include "ShaderProgram.h"
#include "VertexArray.h"

class App
{
public:
	App();
	virtual ~App() {}
	void run();

private:
	std::vector<ShaderProgram> shaderPrograms{};
	std::vector<VertexArray> vertexArrays{};
	Window window{};

	void init();
	void createShaderPrograms();
};

#endif /* end of include guard: APP_H */
