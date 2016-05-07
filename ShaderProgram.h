#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include "Shader.h"
#include "Crashable.h"

class ShaderProgram : public Crashable
{
public:
	const GLuint id{glCreateProgram()};
	ShaderProgram(Shader vert, Shader frag)
	{
		glAttachShader(id, vert.id);
		glAttachShader(id, frag.id);
	}

	void link()
	{
		glLinkProgram(id);

		GLint success;
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (GL_TRUE != success) {
			throw("Program linking failed");
		}
	}

	std::string getInfoLog()
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		return {infoLog};
	}

	void dumpInfoLog(FILE *stream = stderr)
	{
		fprintf(stream, "Shader program building failed!\n%s\n",
		        getInfoLog().c_str());
	}

	void use() { glUseProgram(id); }
	~ShaderProgram() {}
};

#endif /* SHADER_PROGRAM_H */
