#include "ShaderProgram.h"

void ShaderProgram::attachShader(GLuint shaderId) const
{
	if (!glIsShader(shaderId)) throw "This is not a shader!";

	glAttachShader(id, shaderId);
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

void ShaderProgram::getInfoLog(char *infoLog) const
{
	glGetProgramInfoLog(id, 512, NULL, infoLog);
}

void ShaderProgram::use() const { glUseProgram(id); }
