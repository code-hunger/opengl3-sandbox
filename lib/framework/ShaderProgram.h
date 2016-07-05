#ifndef SHADER_PROGRAM_H_IKLFSRJ5
#define SHADER_PROGRAM_H_IKLFSRJ5

#include <GL/glew.h>
#include <string>
#include <unordered_map>

struct ShaderProgram {
	const GLuint id{glCreateProgram()};
	std::unordered_map<const char *, GLint> uniforms;

	void link() const;

	void attachShader(GLuint shaderId) const;

	void getInfoLog(char *infoLog) const; // 512 long

	void use() const;

	GLint loadUniformLocation(const char *);

	void setUniformMatrix(GLint location, const GLfloat *matrix_data);
	void setUniformMatrix(const char *location, const GLfloat *matrix_data);

	/* virtual ~ShaderProgram() {} */

	/* Shader ShaderProgram(const ShaderProgram &) = delete; */
	/* void operator=(const ShaderProgram &) = delete; */
};

#endif /* end of include guard: SHADER_PROGRAM_H_IKLFSRJ5 */
