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

	template <class window_t> window_t acquireWindow() & { return {}; }

	~GlfwWrapper();
};

#endif /* end of include guard: GLFWWRAPPER_H_2OBZJPPD */
