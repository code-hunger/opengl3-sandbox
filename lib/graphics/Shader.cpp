#include "Shader.h"

#include <GL/glew.h>

#include <cstdio>

Shader::Shader(const char* source, const GLenum type)
    : id(glCreateShader(type)), type(type)
{
	glShaderSource(id, 1, &source, nullptr);
}

Shader::~Shader() { glDeleteShader(id); }

bool Shader::tryToCompile()
{
	glCompileShader(id);

	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (GL_TRUE == success) return true;

	glGetShaderInfoLog(id, 512, nullptr, infoLog);
	return false;
}
