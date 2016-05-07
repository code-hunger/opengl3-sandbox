#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

#include <exception>

class Shader
{
public:
	const GLuint id;
	const GLenum type;

	Shader(std::string source, GLenum type): id(glCreateShader(type)), type(type)
	{
        printf("Creating shader!\n");
		const char *str = source.c_str();
		glShaderSource(id, 1, &str, NULL);
        printf("Created shader!\n");
	}

    virtual ~Shader() {
        printf("Shader destruction. Delete shader!\n");
        glDeleteShader(id);
    } // @TODO: should it be virual or not?

	void compile()
	{
        printf("Compiling shader...\n");
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(GL_TRUE != success) {
            dumpInfoLog();
            throw ("Shader compilation failed");
        }
        printf("Shader compiled!\n");
	}

	std::string getInfoLog()
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		return {infoLog};
	}

	void dumpInfoLog(FILE *stream = stderr)
	{
		fprintf(stream, "%s", getInfoLog().c_str());
	}
};

#endif /* SHADER_H */
