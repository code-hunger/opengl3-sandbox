#include "logger.h"

#include <cstdarg>
#include <cstdio>

#define WITH_CONST_DEF(sig, body)                                              \
	const Logger& Logger::operator() sig                                       \
	{                                                                          \
		body;                                                                  \
		return *this;                                                          \
	}                                                                          \
	const Logger& Logger::operator() sig const                                 \
	{                                                                          \
		refresh();                                                             \
		body;                                                                  \
		return *this;                                                          \
	}

WITH_CONST_DEF((bool val), printf("%s", val ? "true" : "false"))

WITH_CONST_DEF((const Color& color), {
	printf("\033[%d;38;5;%dm", color != Color::Default, color);
})

WITH_CONST_DEF((const char* msg, ...), {
	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);
})

void Logger::refresh() const
{
	puts("\033[0m");
	for (int i = 0; i < level; ++i) {
		putchar('\t');
	}
}

Logger& Logger::operator--()
{
	if (level) --level;
	return *this;
}

Logger& Logger::operator++()
{
	if (level < 10) ++level;
	return *this;
}
