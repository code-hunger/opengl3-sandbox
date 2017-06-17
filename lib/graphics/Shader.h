#ifndef SHADER_H_837DGZOX
#define SHADER_H_837DGZOX

#include <string>
using std::string;

struct Shader
{
	unsigned id;
	unsigned type;
	string infoLog;

	Shader(const string& source, unsigned type);
	virtual ~Shader();

	bool tryToCompile();

	Shader(Shader&& other)
	    : id(other.id), type(other.type), infoLog(other.infoLog)
	{
		other.id = 0;
	};
	void operator=(Shader) = delete;
};

std::string getShaderSource(const char* shaderName);

#endif /* end of include guard: SHADER_H_837DGZOX */
