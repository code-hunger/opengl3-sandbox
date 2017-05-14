#ifndef LOGGER_H_F36PNJYA
#define LOGGER_H_F36PNJYA

#include <iostream>

#define LOG Logger::get()
#define WARN Logger::get()(Logger::Orange)
#define ERR Logger::get()(Logger::Red)

#ifndef _MSC_VER
#define PRINTF_FORMAT __attribute__((format(printf, 2, 3)));
#else
#define PRINTF_FORMAT 
#endif

/*
 * NON-const state means indent MUST be printed
 * Const state means indent MUST NOT be printed
 *
 * Result of both states is CONST state
 * Non-const state cat be accuired only by calling Logger::get()
 */

typedef unsigned short ushort;

class Logger
{
	ushort level;

	void refresh() const;

	explicit Logger(char unsigned level = 0) : level(level){};

public:
	enum Color {
		Black = 0,
		Red = 1,
		Green = 10,
		Yellow = 3,
		Orange = 202,
		Blue = 45,
		White = 15,
		Default = -1,
	};

	// printf-like logging; either const or non-const
	const Logger& operator()(const char*, ...) PRINTF_FORMAT;
	const Logger& operator()(const char*, ...) const PRINTF_FORMAT;

	// cout-like logging
	template <typename T> const Logger& operator<<(T);
	template <typename T> const Logger& operator<<(T) const;

	// Color specifiers using (); either const or non-const
	const Logger& operator()(Color);
	const Logger& operator()(Color) const;

	// Color specifiers using <<; either const or non-const
	inline const Logger& operator<<(Color c) { return (*this)(c); };
	inline const Logger& operator<<(Color c) const { return (*this)(c); };

	// Bool overloads; either const or non-const
	const Logger& operator()(bool);
	const Logger& operator()(bool) const;

	// Bool overloads; either const or non-const
	inline const Logger& operator<<(bool b) { return (*this)(b); };
	inline const Logger& operator<<(bool b) const { return (*this)(b); };

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
