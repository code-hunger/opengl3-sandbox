#ifndef LOGGER_H_F36PNJYA
#define LOGGER_H_F36PNJYA

#include <iostream>

#define LOG Logger::get()
#define WARN Logger::get()(Logger::Orange)
#define ERR Logger::get()(Logger::Red)

#ifndef _MSC_VER
#define PRINTF_FORMAT __attribute__((format(printf, 2, 3)))
#else
#define PRINTF_FORMAT
#endif

#define WITH_CONST(sig, body)                                                  \
	const Logger& operator sig body const Logger& operator sig const body

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

	Logger(const Logger&) = delete;
	void operator=(const Logger&) = delete;

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

	// printf-like logging
	WITH_CONST(()(const char*, ...), PRINTF_FORMAT;)

	// cout-like logging
	template <typename T> const Logger& operator<<(T);
	template <typename T> const Logger& operator<<(T) const;

	// Color specifiers
	WITH_CONST(()(const Color&), ;)
	WITH_CONST(<<(const Color& c), { return (*this)(c); })

	// Bool overloads
	WITH_CONST(()(bool), ;)
	WITH_CONST(<<(bool b), { return (*this)(b); })

	// Increase/decrease indentation
	Logger& operator++();
	Logger& operator--();

	// For usage in boolean expressions, e.g. CONDITION && LOG("msg")
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
