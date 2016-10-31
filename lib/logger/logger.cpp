#include "logger.h"

#include <cstdarg>
#include <cstdio>

#include <iostream>

void Logger::operator<<(char x)
{
	if (x == '\n') {
		should_indent = 1;
	}
	putchar(x);
}

Logger& Logger::operator<<(Color color)
{
	printf("\033[%d;%dm", color != Color::Default, color);
	return *this;
}

Logger& Logger::operator()(Color color, const char* msg, ...)
{
	indent();

	printf("\033[1;%dm", color);

	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);

	// reset colors and fonts
	puts("\033[0m");
	return *this;
}

Logger& Logger::operator()(const char* msg, ...)
{
	indent();
	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);
	putchar('\n');
	return *this;
}

void Logger::indent()
{
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

