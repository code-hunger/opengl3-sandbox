#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{
public:
	const GLuint id;
	const GLenum type;

	Shader(const std::string source, const GLenum type);
	virtual ~Shader();

	std::string getInfoLog() const;
	void dumpInfoLog(FILE *stream);
    void compile();

	Shader(const Shader &) = delete;
	void operator=(const Shader &) = delete;
};

#endif /* SHADER_H */
