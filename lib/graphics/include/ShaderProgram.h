#ifndef SHADER_PROGRAM_H_IKLFSRJ5
#define SHADER_PROGRAM_H_IKLFSRJ5

#include <GL/glew.h>
#include <unordered_map>

struct ShaderProgram
{
	const GLuint id{glCreateProgram()};

	// const char* instead of std::string because it's hard-coded in the program
	// and doesn't need any special pointer-handling (like delete[])
	std::unordered_map<const char*, GLint> uniforms;

	void link() const;

	void attachShader(GLuint shaderId) const;

	void createShader(const char* name, GLuint type);

	void getInfoLog(char* infoLog) const; // 512 long

	void use() const;

	GLint loadUniformLocation(const char*);

	void setUniformMatrix(const char* location, const GLfloat* matrix_data);
};

#endif /* end of include guard: SHADER_PROGRAM_H_IKLFSRJ5 */
