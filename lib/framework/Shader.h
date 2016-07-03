#ifndef SHADER_H_837DGZOX
#define SHADER_H_837DGZOX

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

struct Shader {
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

#endif /* end of include guard: SHADER_H_837DGZOX */
