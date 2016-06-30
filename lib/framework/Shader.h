#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Shader
{
public:
	const GLuint id;
	const GLenum type;

	Shader(const std::string source, const GLenum type);
	virtual ~Shader();

	std::string getInfoLog() const;
	void dumpInfoLog();
	void compile();

	Shader(const Shader &) = delete;
	void operator=(const Shader &) = delete;
};

#endif /* SHADER_H */
