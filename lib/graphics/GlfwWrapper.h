#ifndef GLFWWRAPPER_H_2OBZJPPD
#define GLFWWRAPPER_H_2OBZJPPD

class GlfwWrapper
{
	static bool exists;

public:
	GlfwWrapper();

	GlfwWrapper(GlfwWrapper&&) = default;

	GlfwWrapper(const GlfwWrapper&) = delete;
	GlfwWrapper& operator=(GlfwWrapper) = delete;

	~GlfwWrapper();
};

#endif /* end of include guard: GLFWWRAPPER_H_2OBZJPPD */
