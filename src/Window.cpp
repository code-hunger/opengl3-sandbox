#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"

/* #include <assimp/Importer.hpp> */
/* #include <assimp/postprocess.h> */
/* #include <assimp/scene.h> */

#include <cstdlib>

#include <assert.h>

#define WIDTH 766
#define HEIGHT 766

using glm::vec3;
using glm::mat4;
using glm::vec4;
using glm::translate;

// clang-format off
#define float(a) static_cast<float>(a)
// clang-format on
#define setMVP(mvpLoc, mvp)                                                    \
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp))
#define CREATE_WINDOW()                                                        \
	glfwCreateWindow(HEIGHT, WIDTH, "Window Title", NULL, NULL)

bool Window::hintsSet = false;

void Window::setHints()
{
	if (!glfwInit()) {
		throw "Glfw init fail!";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	Window::hintsSet = true;
}

Window::Window() : window(CREATE_WINDOW())
{
	if (!window) {
		throw "A window could not be created!";
	}

	// Window::setHints must be called before constructing a Window, otherwise
	// OpenGL doesn't work
	assert(hintsSet);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		throw "Glew init fail";
	}

	if (!GLEW_VERSION_3_3) {
		throw "No glew_version_3_3!";
	}

	printf("Window constructed!\n");

	glfwSetWindowUserPointer(window, static_cast<void *>(this));

	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
	                              int action, int mods) {
		static_cast<Window *>(glfwGetWindowUserPointer(window))
		    ->keyCallback(key, scancode, action, mods);
	});

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int width, height;
	getSize(&width, &height);
	glViewport(0, 0, width, height);
	glClearColor(1, 1.f, 1, 0);

	try {
		createShaderPrograms();

		// clang-format off
        GLfloat vertices[] = {
            +.5f, +.5f, -.5f, 0.f, 1.f, 0.f, // back    top right // 0
            -.5f, +.5f, -.5f, 1.f, 0.f, 0.f, // back    top left  // 1
            -.5f, -.5f, -.5f, .0f, .0f, 1.f, // back bottom left  // 2
            +.5f, -.5f, -.5f, 0.f, .5f, 0.f, // back bottom right // 3

            -.5f, +.5f,  .5f, .5f, 0.f, 0.f, // face    top left  // 4
            +.5f, +.5f,  .5f, 0.f, .0f, .9f, // face    top right // 5
            -.5f, -.5f, +.5f, 1.f, 1.f, 0.f, // face bottom left  // 6
            +.5f, -.5f, +.5f, 1.f, 1.f, 1.f, // face bottom right // 7
        };

        GLuint trapezoid[] = {
            0, 1, 2, // back
            0, 2, 3, 
            2, 3, 6, // bottom
            3, 6, 7, 
            6, 4, 1, // left
            1, 6, 2,
            5, 7, 0, // right
            0, 7, 3,
            0, 1, 4, // top
            0, 4, 5,
            4, 5, 6, // face
            6, 7, 5,
        };
		// clang-format on

		VertexArray myVertexArray{vertices, sizeof vertices, trapezoid,
		                          sizeof trapezoid};

		myVertexArray.build(3);
		vertexArrays.push_back(myVertexArray);

		// clang-format off
        GLfloat axisVertices[] = {
            .0f, .0f, -10.f,
            .0f, .0f, 10.f,
            .0f, -10.f, .0f,
            .0f, 10.f, 0.f,
            -10.f, .0f, .0f,
            10.f, 0.f, 0.f,
        };
		// clang-format on

		VertexArray axisVertexArray(axisVertices, sizeof axisVertices);
		axisVertexArray.build(3, false);
		vertexArrays.push_back(axisVertexArray);

		// clang-format off
		GLfloat groundVertices[] = {
             100, 0, 0,   0, 0, 1,
             0,   0, 0,   0, 1, 0,
             100, 0, 100, 0, 1, 0,
             100, 0, 100, 0, 1 ,0,
             0,   0, 0,   0, 0, 0,
             0,   0, 100, 0, 1, 0
		};
		// clang-format on

		VertexArray groundVertexArray{groundVertices, sizeof groundVertices};
		groundVertexArray.build(3, true);
		vertexArrays.push_back(groundVertexArray);

	} catch (const char *e) {
		printf("%s\n", e);
		throw - 1;
	}
	printf("App booted successfully!\n");
}

void Window::run()
{
	printf("Start running!\n");
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	int frames = 0;
	double last_time_check = glfwGetTime();
	while (!shouldClose()) {
		double time = glfwGetTime(), deltaTime = time - prevTime;
		prevTime = time;
		++frames;
		if (0 && time - last_time_check >= 1) {
			printf("\t %f since last check", frames / (time - last_time_check));
			last_time_check = glfwGetTime();
			frames = 0;
			int x, y;
			getCursorPos(&x, &y);
			printf("Cursor: %dx%d\n", x, y);
		}
		update(deltaTime);
		render(deltaTime, shaderPrograms, vertexArrays);
	}
	printf("Stop running!\n");
}

void Window::getSize(int *width, int *height) const
{
	glfwGetFramebufferSize(window, width, height);
}

bool Window::shouldClose() const { return glfwWindowShouldClose(window); }

void Window::getCursorPos(int *x, int *y) const
{
	double dx, dy;
	glfwGetCursorPos(window, &dx, &dy);
	*x = static_cast<int>(dx);
	*y = static_cast<int>(dy);
}

void Window::update(double deltaTime)
{
	glfwSwapBuffers(window);
	glfwPollEvents();

	float cameraSpeed = float(deltaTime) * 20;
	mat4 transl;
	if (keys[GLFW_KEY_W] xor keys[GLFW_KEY_S]) {
		float isForward = 0;
		if (keys[GLFW_KEY_W]) {
			isForward = 1;
		} else if (keys[GLFW_KEY_S]) {
			isForward = -1;
		}
		transl =
		    translate(transl, isForward * cameraSpeed * normalize(cameraFront));
	}

	if (keys[GLFW_KEY_D] xor keys[GLFW_KEY_A]) {
		float isRight = 0;
		if (keys[GLFW_KEY_D]) {
			isRight = 1;
		} else if (keys[GLFW_KEY_A]) {
			isRight = -1;
		}
		transl =
		    translate(transl, isRight * cameraSpeed *
		                          normalize(glm::cross(cameraFront, cameraUp)));
	}

	if (keys[GLFW_KEY_J] xor keys[GLFW_KEY_K]) {
		float isUp = 0;
		if (keys[GLFW_KEY_K]) {
			isUp = 1;
		} else if (keys[GLFW_KEY_J]) {
			isUp = -1;
		}
		transl = translate(transl, isUp * cameraSpeed * normalize(cameraUp));
	}

	/* cameraPos = transl * vec4(cameraPos.x, cameraPos.y, cameraPos.z, 1); */

	double _x, _y;
	glfwGetCursorPos(window, &_x, &_y);
	int x = static_cast<int>(_x), y = static_cast<int>(_y);
	int deltaX = x - lastMouseX, deltaY = -y + lastMouseY;

	lastMouseX = x;
	lastMouseY = y;

	GLfloat sensitivity = 3 * float(deltaTime);
	yaw += sensitivity * float(deltaX);
	pitch += sensitivity * float(deltaY);

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	float _yaw = glm::radians(yaw), _pitch = glm::radians(pitch);

	glm::vec3 front;
	front.x = float(cos(_yaw) * cos(_pitch));
	front.y = float(sin(_pitch));
	front.z = float(sin(_yaw) * cos(_pitch));
	cameraFront = glm::normalize(front);
}

void Window::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}

	if (GLFW_PRESS == action) {
		keys[key] = true;
	} else if (GLFW_RELEASE == action) {
		keys[key] = false;
	}
	return;

	printf("Recieved key event! Key: %d, scancode : %d, action : %d, "
	       "mods: %d\n",
	       key, scancode, action, mods);
}

void Window::render(const double deltaTime, const ShaderPrograms &programs,
                    VertexArrays const &vertArrays) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	(void)deltaTime;
	int mvpLoc = glGetUniformLocation(programs[0].id, "model_view_projection");

	int mouseX, mouseY;
	this->getCursorPos(&mouseX, &mouseY);
	int width, height;
	this->getSize(&width, &height);

	mat4 proj, model,
	    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	model = glm::rotate(model, float(glfwGetTime() * 8), vec3(0.8f, 1.0f, .0f));
	proj = glm::perspective(45.f, 1.0f, 0.1f, 250.f);

	programs[0].use();

	srand(42);

	int MAX_CUBE = 100;

	for (int i = 0; i < MAX_CUBE; ++i) {
		mat4 _view = translate(view, vec3(rand() % MAX_CUBE, rand() % MAX_CUBE,
		                                  rand() % MAX_CUBE));
		setMVP(mvpLoc, proj * _view * model);
		vertArrays[0].draw(GL_TRIANGLES, 0);
	}

	setMVP(mvpLoc, proj * view);
	vertArrays[2].draw(GL_TRIANGLES, 0);

	model = rotate(mat4(), glm::radians(90.f), vec3(0, 0, 1));

	setMVP(mvpLoc, proj * view * model);
	vertArrays[2].draw(GL_TRIANGLES, 0);
}

void Window::createShaderPrograms()
{
	printf("Creating shader programms...\n");

	Shader frag_sh(readFile(SHADER_DIRECTORY "/fragment_shader.glsl"),
	               GL_FRAGMENT_SHADER),
	    vert_sh(readFile(SHADER_DIRECTORY "/vertex_shader.glsl"),
	            GL_VERTEX_SHADER);

	frag_sh.compile();
	vert_sh.compile();

	ShaderProgram program{vert_sh.id, frag_sh.id};
	program.link();
	shaderPrograms.push_back(program);

	printf("Shader programs created!\n");
}

Window::~Window()
{
	printf("Window destructor. Destroy window!\n");
	glfwDestroyWindow(window);
}
