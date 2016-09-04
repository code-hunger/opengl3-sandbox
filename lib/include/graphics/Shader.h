#ifndef SHADER_H_837DGZOX
#define SHADER_H_837DGZOX

#include <string>

struct Shader
{
	const unsigned id;
	const unsigned type;

	Shader(std::string source, unsigned type);
	virtual ~Shader();

	void compile();

	Shader(const Shader&) = delete;
	void operator=(const Shader&) = delete;
};

#endif /* end of include guard: SHADER_H_837DGZOX */
