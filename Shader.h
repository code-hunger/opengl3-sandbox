#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{
  public:
	const char *const source;
	const GLenum type;
	const GLuint id;

  public:
	Shader(std::string source, GLenum type)
	    : source(source.c_str()), type(type),
	      id(glCreateShader(type)){glShaderSource(id, 1, &this->source, NULL);}
	Shader(const Shader &);
	void operator=(const Shader &);
	virtual ~Shader(){} // @TODO: should it be virual or not?

	bool compile()
	{
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		return GL_TRUE == success;
	}

	void dumpInfoLog(FILE *stream = stdout)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		fprintf(stream, "%s", infoLog);
	}

	void dumpSource() { printf("%s", source); }
};

#endif /* SHADER_H */
