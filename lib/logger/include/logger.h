#ifndef LOGGER_H_F36PNJYA
#define LOGGER_H_F36PNJYA

#include <cstdio>

#include <iostream>

#define LOG Logger::get()

/*
 * NON-const state means indent MUST be printed
 * Const state means indent MUST NOT be printed
 *
 * Result of both states is CONST state
 * Non-const state cat be accuired only by calling Logger::get()
 */
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

	// printf-like logging; either const or non-const
	const Logger& operator()(const char*, ...)
	    __attribute__((format(printf, 2, 3)));
	const Logger& operator()(const char*, ...) const
	    __attribute__((format(printf, 2, 3)));

	// Color specifiers using (); either const or non-const
	const Logger& operator()(Color);
	const Logger& operator()(Color) const;

	// Color specifiers using <<; either const or non-const
	const Logger& operator<<(Color);
	const Logger& operator<<(Color) const;

	// cout-like logging
	template <typename T> const Logger& operator<<(T);
	template <typename T> const Logger& operator<<(T) const;

	// Increase/decrease indentation
	Logger& operator++();
	Logger& operator--();

	Logger(const Logger&) = delete;
	void operator=(const Logger&) = delete;

	// To be easily used in boolean expressions,
	// e.g. CONDITION && LOG("msg")
	operator bool() const { return 1; };

	static Logger& get()
	{
		static Logger logger;
		return logger;
	}
};

template <typename T> const Logger& Logger::operator<<(T x)
{
	refresh();
	std::cout << x;
	return *this;
}

template <typename T> const Logger& Logger::operator<<(T x) const
{
	std::cout << x;
	return *this;
}

#endif /* end of include guard: LOGGER_H_F36PNJYA */
