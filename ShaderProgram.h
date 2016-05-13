#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include "Shader.h"

class ShaderProgram
{
public:
	const GLuint id{glCreateProgram()};

	ShaderProgram(const Shader &vert, const Shader &frag);

	void link() const;

	std::string getInfoLog();

	void dumpInfoLog(FILE *stream);

	void use();
	~ShaderProgram() {}

	ShaderProgram(const ShaderProgram &) = delete;
	void operator=(const ShaderProgram &) = delete;
};

#endif /* SHADER_PROGRAM_H */
