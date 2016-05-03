#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "utils.h"
#include "ShaderProgram.h"

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

int main()
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

	Shader frag_sh(readFile("fragment_shader.glsl"), GL_FRAGMENT_SHADER),
	    vert_sh(readFile("vertex_shader.glsl"), GL_VERTEX_SHADER);

	if (!frag_sh.compile() || !vert_sh.compile()) {
		return EXIT_FAILURE;
	}

	ShaderProgram shaderProgram(vert_sh, frag_sh);
	shaderProgram.link();

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

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT),
	                      (GLvoid *)(2 * sizeof(GL_FLOAT)));
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

		shaderProgram.use();

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
