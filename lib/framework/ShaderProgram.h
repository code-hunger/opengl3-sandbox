#ifndef SHADER_PROGRAM_H_IKLFSRJ5
#define SHADER_PROGRAM_H_IKLFSRJ5

#include <GL/glew.h>
#include <string>

struct ShaderProgram {
	const GLuint id{glCreateProgram()};

	void link() const;

	void attachShader(GLuint shaderId) const;

	void getInfoLog(char *infoLog) const; // 512 long

	void use() const;
	/* virtual ~ShaderProgram() {} */

	/* Shader ShaderProgram(const ShaderProgram &) = delete; */
	/* void operator=(const ShaderProgram &) = delete; */
};

#endif /* end of include guard: SHADER_PROGRAM_H_IKLFSRJ5 */
