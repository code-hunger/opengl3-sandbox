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
	virtual ~App();
	void run(Window &);

	App(const App &) = delete;
	void operator=(const App &) = delete;

private:
	ShaderPrograms shaderPrograms{};
    VertexArrays vertexArrays{};

	void createShaderPrograms();
};

#endif /* end of include guard: APP_H */
