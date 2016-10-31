#ifndef LOGGER_H_F36PNJYA
#define LOGGER_H_F36PNJYA

#include <cstdio>

#include <iostream>

#define LOG Logger::get()

class Logger
{
	char unsigned level = 0;
	Logger() = default;

	void indent();

	bool should_indent = true;

public:
	enum Color { Red = 31, Green = 32, Blue = 34, Default = 39 };

	Logger& operator()(const char* msg, ...)
	    __attribute__((format(printf, 2, 3)));

	Logger& operator()(Color, const char* msg, ...)
	    __attribute__((format(printf, 3, 4)));

	void operator<<(char x);

	Logger& operator<<(Color);

	template <typename T> Logger& operator<<(T x)
	{
		if (should_indent == 1) indent();
		should_indent = 0;
		std::cout << x;
		return *this;
	}

	// Increase/decrease indentation
	Logger& operator++();
	Logger& operator--();

	// To be easily used in boolean expressions,
	// e.g. CONDITION && LOG("msg")
	operator bool() { return 1; };

	Logger(const Logger&) = delete;
	void operator=(const Logger&) = delete;

	static Logger& get()
	{
		static Logger logger;
		return logger;
	}
};

#endif /* end of include guard: LOGGER_H_F36PNJYA */
