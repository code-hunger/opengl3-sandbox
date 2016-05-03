#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{
  public:
	const GLuint id;
	const GLenum type;

  public:
	Shader(std::string source, GLenum type) : id(glCreateShader(type)), type(type)
	{
		const char *str = source.c_str();
		glShaderSource(id, 1, &str, NULL);
	}

	virtual ~Shader() {} // @TODO: should it be virual or not?

	bool compile()
	{
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		return GL_TRUE == success;
	}

	std::string getInfoLog()
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		return {infoLog};
	}

	void dumpInfoLog(FILE *stream = stderr) { fprintf(stream, "%s", getInfoLog().c_str()); }
};

#endif /* SHADER_H */
