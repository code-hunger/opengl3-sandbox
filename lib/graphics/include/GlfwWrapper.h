#ifndef GLFWWRAPPER_H_2OBZJPPD
#define GLFWWRAPPER_H_2OBZJPPD

#include "Window.h"
#include "logger/include/logger.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <forward_list>

#include <vector>

static void setWindowHints()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef ANTI_ALIASING
	glfwWindowHint(GLFW_SAMPLES, 4);
#endif
}

static void errorCallback(int code, const char* desc)
{
	ERR("GLFW error #%d: %s", code, desc);
	throw "Error callback called! Look at stderr.";
}

class GlfwWrapper
{
	static bool exists;

public:
	GlfwWrapper()
	{
		if (exists) throw "No second glfw wrapper allowed";
		if (!glfwInit()) throw "Glfw init fail!";

		glfwSetErrorCallback(errorCallback);
		setWindowHints();
		exists = true;
	}

	GlfwWrapper(GlfwWrapper&&) = default;

	GlfwWrapper(const GlfwWrapper&) = delete;
	GlfwWrapper& operator=(GlfwWrapper) = delete;

	Window acquireWindow() & { return {}; }

	~GlfwWrapper()
	{
		exists = false;
		glfwTerminate();
		LOG("Wrapper exit!");
	}
};

#endif /* end of include guard: GLFWWRAPPER_H_2OBZJPPD */