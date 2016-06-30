#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include <string>

struct ShaderProgram {
	const GLuint id{glCreateProgram()};

	ShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId);

	void link() const;

	void getInfoLog(char *infoLog); // 512 long

	void use() const;
	~ShaderProgram() {}

	/* ShaderProgram(const ShaderProgram &) = delete; */
	/* void operator=(const ShaderProgram &) = delete; */
};

#endif /* SHADER_PROGRAM_H */
