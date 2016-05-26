#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include <cstdio>
#include <string>

struct ShaderProgram {
	const GLuint id{glCreateProgram()};

	ShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId);

	void link() const;

	std::string getInfoLog();

	void dumpInfoLog(FILE *stream);

	void use() const;
	~ShaderProgram() {}

	/* ShaderProgram(const ShaderProgram &) = delete; */
	/* void operator=(const ShaderProgram &) = delete; */
};

#endif /* SHADER_PROGRAM_H */
