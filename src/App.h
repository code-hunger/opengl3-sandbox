#ifndef APP_H
#define APP_H

#include "ShaderProgram.h"
#include "VertexArray.h"

class Window;
typedef std::vector<ShaderProgram> ShaderPrograms;
typedef std::vector<VertexArray> VertexArrays;

class App
{
public:
	App();
	virtual ~App();
	void run(Window &) const;

	App(const App &) = delete;
	void operator=(const App &) = delete;

private:
	ShaderPrograms shaderPrograms{};
    VertexArrays vertexArrays{};

	void createShaderPrograms();
};

#endif /* end of include guard: APP_H */
