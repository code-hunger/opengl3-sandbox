#ifndef LOGGER_H_F36PNJYA
#define LOGGER_H_F36PNJYA

#include <cstdio>

#include <iostream>

#define LOG Logger::get()

class Logger
{
	char unsigned level;
	bool should_indent;

	void refresh() const;

	Logger(char unsigned level = 0, bool should_indent = true);

public:
	enum Color {
		Black = 30,
		Red = 31,
		Green = 32,
		Yellow = 33,
		Blue = 34,
		White = 37,
		Default = 39,
	};

	const Logger& operator()(const char* msg, ...) const
	    __attribute__((format(printf, 2, 3)));

	const Logger& operator()(const char* msg, ...)
	    __attribute__((format(printf, 2, 3)));

	const Logger& operator()(Color);

	template <typename T> const Logger& operator<<(T x) const
	{
		std::cout << x;
		return *this;
	}

	template <typename T> const Logger& operator<<(T x)
	{
		refresh();
		std::cout << x;
		return *this;
	}

	// Increase/decrease indentation
	Logger& operator++();
	Logger& operator--();

	// To be easily used in boolean expressions,
	// e.g. CONDITION && LOG("msg")
	operator bool() const { return 1; };

	Logger(const Logger&) = delete;
	void operator=(const Logger&) = delete;

	static Logger& get()
	{
		static Logger logger;
		return logger;
	}
};

#endif /* end of include guard: LOGGER_H_F36PNJYA */
