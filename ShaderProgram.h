#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include "Shader.h"
#include "Crashable.h"

class ShaderProgram: public Crashable
{
  public:
	const GLuint id;
	ShaderProgram(Shader vert, Shader frag) : id(glCreateProgram())
	{
		glAttachShader(id, vert.id);
		glAttachShader(id, frag.id);
	}
	bool link()
	{
		glLinkProgram(id);

		GLint success;
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		return GL_TRUE == success;
	}

	std::string getInfoLog()
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		return {infoLog};
	}

	void dumpInfoLog(FILE *stream = stderr)
	{
		fprintf(stream, "Shader program building failed!\n%s\n", getInfoLog().c_str());
	}

	void use() { glUseProgram(id); }
	~ShaderProgram() {}
};

#endif /* SHADER_PROGRAM_H */
