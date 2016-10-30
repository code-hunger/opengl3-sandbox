#ifndef SHADER_H_837DGZOX
#define SHADER_H_837DGZOX

struct Shader
{
	const unsigned id;
	const unsigned type;
	char infoLog[512];

	Shader(const char* source, unsigned type);
	virtual ~Shader();

	bool tryToCompile();

	Shader(const Shader&) = delete;
	void operator=(const Shader&) = delete;
};

#endif /* end of include guard: SHADER_H_837DGZOX */
