#include "Shader.h"
#include <GL/glew.h>

Shader::Shader(const std::string source, const GLenum type)
    : id(glCreateShader(type)), type(type)
{
	printf("Creating shader... ");
	const char *str = source.c_str();
	glShaderSource(id, 1, &str, NULL);
	printf("created!\n");
}

Shader::~Shader()
{
	printf("Shader destruction. Delete shader!\n");
	glDeleteShader(id);
}

void Shader::compile()
{
	printf("Compiling shader... ");
	glCompileShader(id);

	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (GL_TRUE != success) {
		printf("failed!\nInfo log: %s\n", getInfoLog().c_str());
		throw("Shader compilation failed");
	}
	printf("compiled!\n");
}

std::string Shader::getInfoLog() const
{
	GLchar infoLog[512];
	glGetShaderInfoLog(id, 512, NULL, infoLog);
	return {infoLog};
}

void Shader::dumpInfoLog() { fprintf(stderr, "%s", getInfoLog().c_str()); }
