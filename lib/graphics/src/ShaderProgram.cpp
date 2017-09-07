#include "ShaderProgram.h"

#include "Shader.h"
#include "logger/logger.h"

#include <GL/glew.h>

uint ShaderProgram::createShaderProgram() { return glCreateProgram(); }

ShaderProgram::ShaderProgram(const char* shaderName)
{
	createShader(std::string(shaderName).append(".frag").c_str(),
	             GL_FRAGMENT_SHADER);
	createShader(std::string(shaderName).append(".vert").c_str(),
	             GL_VERTEX_SHADER);
}

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

void ShaderProgram::setUniformMatrix(const char* uniformName,
                                     const GLfloat* matrix_data)
{
	use();
	GLint uniformLocation = loadUniformLocation(uniformName);
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, matrix_data);
}

void ShaderProgram::createShader(const char* name, GLuint type)
{
	Shader shader{getShaderSource(name).c_str(), type};

	if (!shader.tryToCompile()) {
		ERR << "\nShader compilation failed: " << shader.infoLog;
		throw "Shader copmilation fail";
	}

	attachShader(shader.id);
}
