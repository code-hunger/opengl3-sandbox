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
	void boot();

private:
	App(const App &);
	void operator=(const App &);

	ShaderProgram *shpr;
	VertexArray *var;
	std::vector<ShaderProgram> shaderPrograms{};
	std::vector<VertexArray> vertexArrays{};

	void createShaderPrograms();
};

#endif /* end of include guard: APP_H */
