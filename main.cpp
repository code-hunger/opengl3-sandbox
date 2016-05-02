#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstdio>
#include <string>
#include <fstream>
#include <streambuf>
#include <cmath>

bool change_color = true;

void error_callback(int error, const char *desc) { printf("Error #%d: %s\n", error, desc); }

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) change_color = !change_color;

	printf("Recieved key event! Key: %d, scancode: %d, action: %d, "
	       "mods: %d\n",
	       key, scancode, action, mods);
}

GLuint loadAShader(const char *shaderSource, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	GLint success;
	GLchar infoLog[512];

	glShaderSource(shader, 1, &shaderSource, NULL);

	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stderr, "shader compilation failed!\n%s\n", infoLog);
		throw "Shader compilation failed!";
	}
	return shader;
}

void loadShaders(GLuint *frag_shader, GLuint *vert_shader)
{
	const char *frag_shader_source = "#version 330 core\n\
                                      out vec4 color;\
                                      in vec3 vertexColor;\
                                      void main() {\
                                          color = vec4(vertexColor, 1.0f);\
                                      } \0",
	           *vert_shader_source = "#version 330 core\n\
                                      layout (location = 0) in vec2 position;\
                                      layout (location = 1) in vec3 color;\
                                      out vec3 vertexColor;\
                                      void main() { \
                                          gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);\
                                          vertexColor = color;\
                                      } \0";
	*frag_shader = loadAShader(frag_shader_source, GL_FRAGMENT_SHADER);
	*vert_shader = loadAShader(vert_shader_source, GL_VERTEX_SHADER);
}

void initGlfw()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

GLuint loadShaderProgram(GLuint frag_shader, GLuint vert_shader)
{
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vert_shader);
	glAttachShader(shaderProgram, frag_shader);
	glLinkProgram(shaderProgram);

	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "program building failed!\n%s\n", infoLog);
		throw "Program building failed!";
	}

	glDeleteShader(frag_shader);
	glDeleteShader(vert_shader);

	return shaderProgram;
}

int main()
{
	initGlfw();

	GLFWwindow *window = glfwCreateWindow(640, 480, "Title", NULL, NULL);

	if (!window) {
		glfwTerminate();
		printf("Window not created!\n");
		return (EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("Glew initialization failed!\n");
		return EXIT_FAILURE;
	}

	GLuint frag_shader, vert_shader;
	loadShaders(&frag_shader, &vert_shader);
	GLuint shaderProgram = loadShaderProgram(frag_shader, vert_shader);

	// clang-format off
	GLfloat vertices[] = {
	    -0.5f,  -0.5f, .0f, .3f, .8f,
	    -0.3f,   0.5f, .8f, 0.f, .5f,
	     0.5f,   0.5f, 1.f, 1.f, 1.f,
	     0.5f,  -0.5f, 1.f, 1.f, 0.f,
	    -0.75f,  0.f,  .0f, .5f, .5f
	};
	// clang-format on

	GLuint indices[] = {0, 1, 3, 4, 1, 2, 4, 1, 0};

	GLuint VAO[2], VBO[2], EBO;
	glGenVertexArrays(2, VAO);

	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)(2 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // unbind

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // or GL_LINES or GL_POINT

	glfwSetKeyCallback(window, key_callback);

	/* GLint loc = glGetUniformLocation(shaderProgram, "vertexColor"); */

	while (!glfwWindowShouldClose(window)) {
		int width, height;
		/* double time = glfwGetTime(); */
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0.f, 0, 0);

		/* if (change_color) { */
		/* 	float green = (float)(sin(time) / 2) + 0.5f; */
		/* 	glUniform4f(loc, (green / 2 + 0.5f), green, 1 - green, 1.f); */
		/* } */

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0); // unbind

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	printf("Exit!\n");

	return EXIT_SUCCESS;
}
