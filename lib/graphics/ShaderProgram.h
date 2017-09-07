#ifndef SHADER_PROGRAM_H_IKLFSRJ5
#define SHADER_PROGRAM_H_IKLFSRJ5

#include <unordered_map>

typedef unsigned uint;

struct ShaderProgram
{
	static uint createShaderProgram();

	const uint id = createShaderProgram();

	ShaderProgram() = default;
	ShaderProgram(const char* shaderName);

	// const char* instead of std::string because it's hard-coded in the program
	// and doesn't need any special pointer-handling (like delete[])
	std::unordered_map<const char*, int> uniforms{};

	void link() const;

	void attachShader(uint shaderId) const;

	void createShader(const char* name, uint type);

	void getInfoLog(char* infoLog) const; // 512 long

	void use() const;

	int loadUniformLocation(const char*);

	void setUniformMatrix(const char* location, const float* matrix_data);
};

#endif /* end of include guard: SHADER_PROGRAM_H_IKLFSRJ5 */
