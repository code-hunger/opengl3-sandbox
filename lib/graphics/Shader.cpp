#include "Shader.h"
#include "logger/include/logger.h"

#include <GL/glew.h>

#include <cstdio>

Shader::Shader(const string& source, const GLenum type)
    : id(glCreateShader(type)), type(type), infoLog()
{
	const char* data = source.c_str();
	glShaderSource(id, 1, &data, nullptr);
}

Shader::~Shader() { glDeleteShader(id); }

std::string getInfoLog(GLuint shaderId)
{
	GLsizei logLength;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

	char* infoLog = new char[static_cast<unsigned>(logLength)];

	glGetShaderInfoLog(shaderId, 512, &logLength, infoLog);
	return infoLog;
}

bool Shader::tryToCompile()
{
	glCompileShader(id);

	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (GL_TRUE == success) return true;

	infoLog = getInfoLog(id);

	return false;
}
