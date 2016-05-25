#ifndef APP_H
#define APP_H

#include <vector>
#include "ShaderProgram.h"
#include "VertexArray.h"

#include <memory>
#include <vector>

class Window;
typedef std::vector<std::unique_ptr<ShaderProgram>> ShaderPrograms;
typedef std::vector<std::unique_ptr<VertexArray>> VertexArrays;

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
