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

void ShaderProgram::getInfoLog(char* infoLog) const
{
	glGetProgramInfoLog(id, 512, NULL, infoLog);
}

void ShaderProgram::use() const { glUseProgram(id); }

GLint ShaderProgram::loadUniformLocation(const char* uniform)
{
	if (uniforms.count(uniform)) return uniforms[uniform];

	GLint loc = glGetUniformLocation(id, uniform);
	if (loc < 0) {
		throw "Uniform location not found!";
	}
	uniforms[uniform] = loc;
	return loc;
}

void ShaderProgram::setUniformMatrix(GLint location, const GLfloat* matrix_data)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, matrix_data);
}

void ShaderProgram::setUniformMatrix(const char* location,
                                     const GLfloat* matrix_data)
{
	setUniformMatrix(loadUniformLocation(location), matrix_data);
}
