#include "Shader.h"
#include <GL/glew.h>

Shader::Shader(const std::string source, const GLenum type)
    : id(glCreateShader(type)), type(type)
{
	const char* str = source.c_str();
	glShaderSource(id, 1, &str, nullptr);
}

Shader::~Shader() { glDeleteShader(id); }

void Shader::compile()
{
	glCompileShader(id);

	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (GL_TRUE != success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(id, 512, nullptr, infoLog);

		printf("Info log: %s\n", infoLog);
		throw("Shader compilation failed");
	}
}
