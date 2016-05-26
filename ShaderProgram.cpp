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

std::string ShaderProgram::getInfoLog()
{
	GLchar infoLog[512];
	glGetProgramInfoLog(id, 512, NULL, infoLog);
	return {infoLog};
}

void ShaderProgram::dumpInfoLog(FILE *stream = stderr)
{
	fprintf(stream, "Shader program building failed!\n%s\n",
	        getInfoLog().c_str());
}

void ShaderProgram::use() const { glUseProgram(id); }
