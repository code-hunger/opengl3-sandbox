#include "Shader.h"

Shader::Shader(const std::string source, const GLenum type)
    : id(glCreateShader(type)), type(type)
{
	printf("Creating shader!\n");
	const char *str = source.c_str();
	glShaderSource(id, 1, &str, NULL);
	printf("Created shader!\n");
}

Shader::~Shader()
{
	printf("Shader destruction. Delete shader!\n");
	glDeleteShader(id);
} // @TODO: should it be virual or not?

void Shader::compile()
{
	printf("Compiling shader...\n");
	glCompileShader(id);

	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (GL_TRUE != success) {
		dumpInfoLog(stderr);
		throw("Shader compilation failed");
	}
	printf("Shader compiled!\n");
}

std::string Shader::getInfoLog() const
{
	GLchar infoLog[512];
	glGetShaderInfoLog(id, 512, NULL, infoLog);
	return {infoLog};
}

void Shader::dumpInfoLog(FILE *stream)
{
	fprintf(stream, "%s", getInfoLog().c_str());
}
