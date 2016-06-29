#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);
}

void ShaderProgram::link() const
{
	glLinkProgram(id);

	GLint success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (GL_TRUE != success) {
		throw("Program linking failed");
	}
}

void ShaderProgram::getInfoLog(char* infoLog)
{
	glGetProgramInfoLog(id, 512, NULL, infoLog);
}

void ShaderProgram::use() const { glUseProgram(id); }
