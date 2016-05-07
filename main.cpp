#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Crashable.h"
#include "CrashHandler.h"
#include "App.h"

int main()
{
    try {
        throw "DAda";
    } catch (const char* a) {
        printf("caught! %s\n", a);
    }

    App app;

	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // or GL_LINES or GL_POINT

	glfwTerminate();

	return EXIT_SUCCESS;
}
